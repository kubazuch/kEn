#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <limits>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include <kEn/core/log.hpp>

/** @file
 *  @ingroup ken
 */

// Adapted from https://github.com/gizmokis/resin/pull/22 for slot map structure
// migoox, 04.12.2024

namespace kEn {

template <typename T>
class Registry;

template <typename T>
struct Id;

template <typename T>
struct IdView;

/**
 * @brief Generational index that identifies a slot in a @ref Registry.
 *
 * A handle is considered addressing a slot when @c index is not @c kInvalidIndex.
 * The @c generation field must match the registry slot's own generation counter
 * for the handle to be live; mismatches indicate a recycled slot.
 *
 * @tparam T The object type managed by the associated registry.
 */
template <typename T>
struct Handle {
  using object_type = T;

  /** @brief Sentinel index value indicating an unoccupied or invalidated handle. */
  static constexpr uint32_t kInvalidIndex = std::numeric_limits<uint32_t>::max();

  uint32_t index      = kInvalidIndex;
  uint32_t generation = 0;

  /** @brief Returns true if this handle addresses a slot (index is not @c kInvalidIndex). */
  [[nodiscard]] constexpr bool is_valid() const noexcept { return index != kInvalidIndex; }
  constexpr auto operator<=>(const Handle&) const noexcept = default;
};

/**
 * @brief Fixed-capacity generational slot map that issues and tracks handles for objects of type @p T.
 *
 * The registry does not own the objects it references; callers are responsible for object
 * lifetime. Binding a pointer via @ref bind() does not transfer ownership.
 *
 * Slot acquisition, binding, and release are private. @p T is granted access to these
 * methods through the @c friend T declaration inside the registry.
 *
 * @tparam T The managed object type.
 */
template <typename T>
class Registry {
 public:
  using object_type = T;
  using handle_type = Handle<T>;

  Registry() = delete;
  DELETE_COPY_MOVE(Registry);

  /**
   * @brief Constructs a registry pre-populated with @p capacity free slots.
   * @param capacity Total number of slots. Fixed for the lifetime of the registry.
   */
  explicit Registry(uint32_t capacity) : capacity_(capacity) {
    slots_.resize(capacity_);
    free_.reserve(capacity_);
    for (uint32_t i = capacity_; i > 0; --i) {
      free_.push_back(i - 1);
    }
  }

  /**
   * @brief Returns true if @p h refers to a currently live slot.
   *
   * Checks that the index is in range and that the handle generation matches the slot's
   * current generation counter. A handle whose slot has been recycled returns false.
   */
  [[nodiscard]] bool valid(handle_type h) const noexcept {
    return h.is_valid() && h.index < capacity_ && slots_[h.index].generation == h.generation;
  }

  /**
   * @brief Returns the object bound to @p h, or @c nullptr if the handle is not live.
   * @param h Handle to look up.
   * @return Pointer to the bound object, or @c nullptr if @p h is invalid or expired.
   */
  [[nodiscard]] T* get(handle_type h) const noexcept {
    if (!valid(h)) {
      return nullptr;
    }
    return slots_[h.index].ptr;
  }

  /**
   * @brief Looks up a live slot by raw index, bypassing the generation check.
   *
   * Returns the @ref IdView for whichever object currently occupies slot @p raw_id,
   * regardless of which @ref Id originally claimed that slot. Returns @c std::nullopt
   * if @p raw_id is out of range or the slot is unbound.
   *
   * @param raw_id Slot index to query.
   * @return The view for the current occupant of the slot, or @c std::nullopt.
   *
   * @note This is intended for serialization or editor lookups. Prefer @ref get() with
   *       a typed @ref Handle when generation safety is required.
   */
  [[nodiscard]] std::optional<IdView<T>> from_raw_id(uint32_t raw_id) const noexcept {
    if (raw_id >= capacity_) {
      return std::nullopt;
    }

    const Slot& slot = slots_[raw_id];
    if (slot.ptr == nullptr) {
      return std::nullopt;
    }

    return IdView<T>({raw_id, slot.generation}, *this);
  }

  /** @brief Returns the total number of slots in this registry. */
  [[nodiscard]] uint32_t capacity() const noexcept { return capacity_; }

  /** @brief Logs an error if any handles are still live at destruction. */
  ~Registry() {
    if (free_.size() != capacity_) {
      KEN_CORE_ERROR("Registry<{}> destroyed with {} live handles.", typeid(T).name(), capacity_ - free_.size());
    }
  }

 private:
  friend T;
  friend struct Id<T>;

  struct Slot {
    uint32_t generation = 0;
    T* ptr              = nullptr;
  };

  /** @brief Pops a free slot and returns a handle for it. @throws std::runtime_error if no slots remain. */
  [[nodiscard]] handle_type acquire() {
    if (free_.empty()) {
      KEN_CORE_CRITICAL("No free handles available in Registry<{}>.", typeid(T).name());
      throw std::runtime_error("No free handles available in Registry.");
    }

    uint32_t idx = free_.back();
    free_.pop_back();
    return {idx, slots_[idx].generation};
  }

  /** @brief Stores @p obj as the bound pointer for handle @p h. Warns and no-ops on invalid handle or null @p obj. */
  void bind(handle_type h, T* obj) noexcept {
    if (!valid(h)) {
      KEN_CORE_WARN("Tried to bind invalid handle ({}, {}) in Registry<{}>.", h.index, h.generation, typeid(T).name());
      return;
    }
    if (obj == nullptr) {
      KEN_CORE_WARN("Tried to bind nullptr to handle ({}, {}) in Registry<{}>.", h.index, h.generation,
                    typeid(T).name());
      return;
    }

    slots_[h.index].ptr = obj;
  }

  /** @brief Unbinds the object pointer, increments the slot generation, and returns the slot to the free list. */
  void release(handle_type h) noexcept {
    if (!valid(h)) {
      KEN_CORE_WARN("Tried to release invalid handle ({}, {}) from Registry<{}>.", h.index, h.generation,
                    typeid(T).name());
      return;
    }

    slots_[h.index].ptr = nullptr;
    ++slots_[h.index].generation;
    free_.push_back(h.index);
  }

  std::vector<Slot> slots_;
  std::vector<uint32_t> free_;
  uint32_t capacity_;
};

/**
 * @brief RAII owning handle that acquires a registry slot on construction and releases it on destruction.
 *
 * @c Id is non-copyable. Moving an @c Id transfers ownership; the moved-from @c Id becomes
 * invalid and must not be used except to be destroyed or reassigned.
 *
 * @tparam T The managed object type.
 */
template <typename T>
struct Id {
  using object_type = T;
  using handle_type = Handle<T>;

  Id() = delete;

  /**
   * @brief Acquires a slot from @p registry.
   * @param registry The registry to acquire from.
   * @throws std::runtime_error if the registry has no free slots.
   */
  explicit Id(Registry<T>& registry) : registry_(&registry), handle_(registry.acquire()) {}

  /** @brief Releases the acquired slot back to the registry. */
  ~Id() { reset(); }

  Id(const Id<T>&)               = delete;
  Id<T>& operator=(const Id<T>&) = delete;

  /** @brief Transfers ownership from @p other; @p other becomes invalid. */
  Id(Id<T>&& other) noexcept
      : registry_(std::exchange(other.registry_, nullptr)), handle_(std::exchange(other.handle_, handle_type{})) {}

  /** @brief Releases the current slot, then transfers ownership from @p other. */
  Id<T>& operator=(Id<T>&& other) noexcept {
    if (this != &other) {
      reset();
      registry_ = std::exchange(other.registry_, nullptr);
      handle_   = std::exchange(other.handle_, handle_type{});
    }
    return *this;
  }

  [[nodiscard]] constexpr bool operator==(const Id<T>& other) const noexcept { return handle_ == other.handle_; }

  /** @brief Returns the underlying @ref Handle. */
  [[nodiscard]] constexpr handle_type handle() const noexcept { return handle_; }

  /** @brief Returns the raw slot index. */
  [[nodiscard]] constexpr uint32_t raw_id() const noexcept { return handle_.index; }

  /** @brief Returns true if this @c Id holds a valid (non-sentinel) handle. */
  [[nodiscard]] constexpr bool is_valid() const noexcept { return handle_.is_valid(); }

  /** @brief Returns a reference to the associated registry. */
  Registry<T>& registry() noexcept { return *registry_; }

  /** @copydoc registry() */
  const Registry<T>& registry() const noexcept { return *registry_; }

 private:
  void reset() noexcept {
    if (registry_ != nullptr && handle_.is_valid()) {
      registry_->release(handle_);
    }
    registry_ = nullptr;
    handle_   = {};
  }

  Registry<T>* registry_{};
  handle_type handle_{};
};

/**
 * @brief Non-owning, potentially stale view of an object in a @ref Registry.
 *
 * An @c IdView may outlive the @ref Id it was constructed from. Call @ref expired()
 * before dereferencing via @ref get() to confirm the referenced slot is still live.
 *
 * @tparam T The managed object type.
 */
template <typename T>
struct IdView {
  using object_type = T;
  using handle_type = Handle<T>;

  IdView() = delete;

  /**
   * @brief Constructs a view from an explicit handle and registry.
   * @param h        Handle to observe.
   * @param registry Registry that owns the slot.
   */
  IdView(handle_type h, const Registry<T>& registry) noexcept  // NOLINT(google-explicit-constructor)
      : handle_(h), registry_(&registry) {}

  /**
   * @brief Constructs a view from an owning @ref Id.
   * @param id The @c Id to observe.
   */
  IdView(const Id<T>& id) noexcept  // NOLINT(google-explicit-constructor)
      : handle_(id.handle()), registry_(&id.registry()) {}

  [[nodiscard]] constexpr bool operator==(const IdView<T>&) const noexcept = default;

  /** @brief Returns the underlying @ref Handle. */
  [[nodiscard]] constexpr handle_type handle() const noexcept { return handle_; }

  /** @brief Returns the raw slot index. */
  [[nodiscard]] constexpr uint32_t raw_id() const noexcept { return handle_.index; }

  /**
   * @brief Returns the bound object pointer, or @c nullptr if the handle is no longer live.
   * @pre The view must have been constructed from a valid @ref Id or registry reference.
   */
  [[nodiscard]] T* get() const noexcept { return registry_->get(handle_); }

  /** @brief Returns true if this view holds a non-sentinel handle index. Does not check liveness. */
  [[nodiscard]] constexpr bool is_valid() const noexcept { return handle_.is_valid(); }

  /**
   * @brief Returns true if the handle is invalid or the referenced slot has been recycled.
   *
   * A view is expired when the slot generation no longer matches, which happens after the
   * originating @ref Id is destroyed and the slot is released back to the registry.
   */
  [[nodiscard]] bool expired() const noexcept { return !is_valid() || !registry_->valid(handle_); }

  /** @brief Returns a reference to the associated registry. */
  [[nodiscard]] const Registry<T>& registry() const noexcept { return *registry_; }

 private:
  handle_type handle_{};
  const Registry<T>* registry_{};
};

}  // namespace kEn

namespace std {

/** @brief @c std::hash specialization for @ref kEn::Handle. */
template <typename T>
struct hash<kEn::Handle<T>> {
  /** @brief Hashes by combining generation and index into a single 64-bit value. */
  [[nodiscard]] size_t operator()(kEn::Handle<T> h) const noexcept {
    return hash<uint64_t>{}((static_cast<uint64_t>(h.generation) << 32) | h.index);
  }
};

/** @brief @c std::hash specialization for @ref kEn::IdView. */
template <typename T>
struct hash<kEn::IdView<T>> {
  /** @brief Delegates to @c std::hash<kEn::Handle<T>>. */
  [[nodiscard]] size_t operator()(const kEn::IdView<T>& id) const noexcept {
    return hash<kEn::Handle<T>>{}(id.handle());
  }
};

}  // namespace std
