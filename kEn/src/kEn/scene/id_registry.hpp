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

// Adapted from https://github.com/gizmokis/resin/pull/22 for slot map structure
// migoox, 04.12.2024

namespace kEn {

template <typename T>
class Registry;

template <typename T>
struct Id;

template <typename T>
struct IdView;

template <typename T>
struct Handle {
  using object_type = T;

  static constexpr uint32_t kInvalidIndex = std::numeric_limits<uint32_t>::max();

  uint32_t index      = kInvalidIndex;
  uint32_t generation = 0;

  [[nodiscard]] constexpr bool is_valid() const noexcept { return index != kInvalidIndex; }
  constexpr auto operator<=>(const Handle&) const noexcept = default;
};

template <typename T>
class Registry {
 public:
  using object_type = T;
  using handle_type = Handle<T>;

  Registry() = delete;
  DELETE_COPY_MOVE(Registry);

  explicit Registry(uint32_t capacity) : capacity_(capacity) {
    slots_.resize(capacity_);
    free_.reserve(capacity_);
    for (uint32_t i = capacity_; i > 0; --i) {
      free_.push_back(i - 1);
    }
  }

  [[nodiscard]] bool valid(handle_type h) const noexcept {
    return h.is_valid() && h.index < capacity_ && slots_[h.index].generation == h.generation;
  }

  [[nodiscard]] T* get(handle_type h) const noexcept {
    if (!valid(h)) {
      return nullptr;
    }
    return slots_[h.index].ptr;
  }

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

  [[nodiscard]] uint32_t capacity() const noexcept { return capacity_; }

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

  [[nodiscard]] handle_type acquire() {
    if (free_.empty()) {
      KEN_CORE_CRITICAL("No free handles available in Registry<{}>.", typeid(T).name());
      throw std::runtime_error("No free handles available in Registry.");
    }

    uint32_t idx = free_.back();
    free_.pop_back();
    return {idx, slots_[idx].generation};
  }

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

template <typename T>
struct Id {
  using object_type = T;
  using handle_type = Handle<T>;

  Id() = delete;

  explicit Id(Registry<T>& registry) : registry_(&registry), handle_(registry.acquire()) {}

  ~Id() { reset(); }

  Id(const Id<T>&)               = delete;
  Id<T>& operator=(const Id<T>&) = delete;

  Id(Id<T>&& other) noexcept
      : registry_(std::exchange(other.registry_, nullptr)), handle_(std::exchange(other.handle_, handle_type{})) {}

  Id<T>& operator=(Id<T>&& other) noexcept {
    if (this != &other) {
      reset();
      registry_ = std::exchange(other.registry_, nullptr);
      handle_   = std::exchange(other.handle_, handle_type{});
    }
    return *this;
  }

  [[nodiscard]] constexpr bool operator==(const Id<T>& other) const noexcept { return handle_ == other.handle_; }

  [[nodiscard]] constexpr handle_type handle() const noexcept { return handle_; }

  [[nodiscard]] constexpr uint32_t raw_id() const noexcept { return handle_.index; }

  [[nodiscard]] constexpr bool is_valid() const noexcept { return handle_.is_valid(); }

  Registry<T>& registry() noexcept { return *registry_; }

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

template <typename T>
struct IdView {
  using object_type = T;
  using handle_type = Handle<T>;

  IdView() = delete;
  IdView(handle_type h, const Registry<T>& registry) noexcept  // NOLINT(google-explicit-constructor)
      : handle_(h), registry_(&registry) {}
  IdView(const Id<T>& id) noexcept  // NOLINT(google-explicit-constructor)
      : handle_(id.handle()), registry_(&id.registry()) {}

  [[nodiscard]] constexpr bool operator==(const IdView<T>&) const noexcept = default;

  [[nodiscard]] constexpr handle_type handle() const noexcept { return handle_; }

  [[nodiscard]] constexpr uint32_t raw_id() const noexcept { return handle_.index; }

  [[nodiscard]] T* get() const noexcept { return registry_->get(handle_); }

  [[nodiscard]] constexpr bool is_valid() const noexcept { return handle_.is_valid(); }

  [[nodiscard]] bool expired() const noexcept { return !is_valid() || !registry_->valid(handle_); }

  [[nodiscard]] const Registry<T>& registry() const noexcept { return *registry_; }

 private:
  handle_type handle_{};
  const Registry<T>* registry_{};
};

}  // namespace kEn

namespace std {

template <typename T>
struct hash<kEn::Handle<T>> {
  [[nodiscard]] size_t operator()(kEn::Handle<T> h) const noexcept {
    return hash<uint64_t>{}((static_cast<uint64_t>(h.generation) << 32) | h.index);
  }
};

template <typename T>
struct hash<kEn::IdView<T>> {
  [[nodiscard]] size_t operator()(const kEn::IdView<T>& id) const noexcept {
    return hash<kEn::Handle<T>>{}(id.handle());
  }
};

}  // namespace std
