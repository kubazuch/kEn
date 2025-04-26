#pragma once

#include <cstddef>
#include <functional>
#include <kEn/core/log.hpp>
#include <limits>
#include <stack>
#include <type_traits>

// Adapted from https://github.com/gizmokis/resin/pull/22
// migoox, 04.12.2024

namespace kEn {

template <typename T>
class IdRegistry {
 public:
  IdRegistry() = delete;
  VIRTUAL_FIVE(IdRegistry);

  explicit IdRegistry(size_t size) : max_objs_(size) {
    for (size_t i = size; i > 0; --i) {
      free_.push(i - 1);
    }

    ids_.resize(size, false);
  }

  size_t register_id() {
    if (free_.empty()) {
      KEN_CORE_CRITICAL("No free ids available in IdRegistry.");
      throw std::runtime_error("No free ids available in IdRegistry.");
    }

    size_t new_id = free_.top();
    free_.pop();
    ids_[new_id] = true;

    return new_id;
  }

  bool is_registered(size_t id) const {
    if (id >= max_objs_) {
      return false;
    }

    return ids_[id];
  }

  size_t get_max_objs() const { return max_objs_; }

  void unregister_id(size_t id) {
    if (id >= max_objs_) {
      KEN_CORE_WARN("Tried to unregister id {} out of range of IdRegistry<{}> [{}].", id, typeid(T).name(), max_objs_);
      return;
    }

    ids_[id] = false;
    free_.push(id);
  }

  ~IdRegistry() {
    if (free_.size() != max_objs_) {
      KEN_CORE_ERROR("IdRegistry<{}> destroyed with {} unregistered objects.", typeid(T).name(),
                     max_objs_ - free_.size());
    }
  }

 private:
  std::stack<size_t> free_;
  std::vector<bool> ids_;
  size_t max_objs_;
};

template <typename T>
struct Id {
  using object_type = T;

  Id() : raw_id_(std::numeric_limits<size_t>::max()) {}
  explicit Id(IdRegistry<T>& registry) : registry_(registry) { raw_id_ = registry_.get().register_id(); }

  ~Id() {
    if (raw_id_ != std::numeric_limits<size_t>::max()) {
      registry_.get().unregister_id(raw_id_);
    }
  }

  Id(const Id<T>& other)               = delete;
  Id<T>& operator=(const Id<T>& other) = delete;

  Id(Id<T>&& other) noexcept : raw_id_(other.raw_id_), registry_(other.registry_) {
    other.raw_id_ = std::numeric_limits<size_t>::max();
  }

  Id<T>& operator=(Id<T>&& other) noexcept {
    if (this != &other) {
      if (raw_id_ != std::numeric_limits<size_t>::max()) {
        registry_.get().unregister_id(raw_id_);
      }
      raw_id_       = other.raw_id_;
      registry_     = other.registry_;
      other.raw_id_ = std::numeric_limits<size_t>::max();
    }
    return *this;
  }

  bool operator==(const Id<T>& other) const { return raw_id_ == other.raw_id_; }
  bool operator!=(const Id<T>& other) const { return raw_id_ != other.raw_id_; }

  inline size_t raw_id() const { return raw_id_; }
  inline bool is_valid() const { return raw_id_ != std::numeric_limits<size_t>::max(); }
  inline bool expired() const { return !registry_.get().is_registered(raw_id_); }

  const IdRegistry<T>& registry() const { return registry_.get(); }

 private:
  size_t raw_id_;
  std::reference_wrapper<IdRegistry<T>> registry_;
};

template <typename IdType>
  requires std::is_base_of_v<Id<typename IdType::object_type>, IdType>
struct IdView {
  IdView() = delete;
  IdView(const IdType& id) : raw_id_(id.raw_id()), registry_(id.registry()) {}  // NOLINT

  bool operator==(const IdView<IdType>& other) const { return raw_id_ == other.raw_id_; }
  bool operator!=(const IdView<IdType>& other) const { return raw_id_ != other.raw_id_; }
  bool operator==(const IdType& other) const { return raw_id_ == other.raw(); }
  bool operator!=(const IdType& other) const { return raw_id_ != other.raw(); }

  inline size_t raw_id() const { return raw_id_; }
  inline bool is_valid() const { return raw_id_ != std::numeric_limits<size_t>::max(); }
  inline bool expired() const { return !registry_.get().is_registered(raw_id_); }

 private:
  size_t raw_id_;
  std::reference_wrapper<const IdRegistry<typename IdType::object_type>> registry_;
};

template <typename T>
struct IdViewHash {
  [[nodiscard]] size_t operator()(const IdView<T>& id) const { return std::hash<size_t>()(id.raw_id()); }
};

}  // namespace kEn