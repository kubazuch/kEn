#pragma once
#include <deque>
#include <initializer_list>
#include <unordered_set>

namespace kEn {  // NOLINT

template <typename T>
class DequeSet {
 public:
  using iterator       = typename std::deque<T>::iterator;
  using const_iterator = typename std::deque<T>::const_iterator;

  DequeSet() = default;
  DequeSet(std::initializer_list<T> init_list) {
    for (const auto& item : init_list) {
      insert(item);
    }
  }

  iterator begin() { return deque_.begin(); }
  iterator end() { return deque_.end(); }
  const_iterator begin() const { return deque_.begin(); }
  const_iterator end() const { return deque_.end(); }

  const T& front() const { return deque_.front(); }
  const T& back() const { return deque_.back(); }

  size_t count(const T& item) const { return set_.count(item); }
  bool contains(const T& item) const { return set_.contains(item); }

  bool empty() const { return set_.empty(); }
  size_t size() const { return set_.size(); }

  void insert(const T& item) {
    if (set_.insert(item).second) {
      deque_.push_back(item);
    }
  }

  bool erase(const T& item) {
    if (set_.erase(item)) {
      auto it = std::find(deque_.begin(), deque_.end(), item);
      if (it != deque_.end()) {
        deque_.erase(it);
      }
      return true;
    }
    return false;
  }

  void clear() {
    set_.clear();
    deque_.clear();
  }

 private:
  std::deque<T> deque_;
  std::unordered_set<T> set_;
};

}  // namespace kEn