#pragma once

#include <algorithm>
#include <deque>
#include <initializer_list>
#include <unordered_set>
#include <utility>

/** @file
 *  @ingroup ken
 */

namespace kEn {  // NOLINT

/**
 * @brief An insertion-ordered set.
 *
 * `DequeSet` behaves like a set with deterministic iteration order (insertion order).
 * Duplicate inserts are ignored.
 *
 * ### Complexity
 * - contains / count: average O(1)
 * - insert / emplace: average O(1)
 * - iteration: O(n)
 * - erase(value): O(n) (search in deque) + average O(1) (erase in set)
 *
 * ### Exception safety
 * - `insert/emplace` provide a strong guarantee: on failure the container remains unchanged.
 * - `erase` attempts to avoid desynchronizing internal structures by mutating the deque first.
 *
 * @tparam T Element type. Must be hashable and equality comparable for `std::unordered_set<T>`.
 *
 * @warning Not thread-safe. Concurrent access must be externally synchronized.
 */
template <typename T>
class DequeSet {
 public:
  /// Container type that defines iteration order.
  using order_container = std::deque<T>;
  /// Container type that provides uniqueness / fast membership tests.
  using set_container = std::unordered_set<T>;

  using value_type     = T;
  using size_type      = typename order_container::size_type;
  using iterator       = typename order_container::iterator;
  using const_iterator = typename order_container::const_iterator;

  /**
   * @brief Constructs an empty DequeSet.
   */
  DequeSet() = default;

  /**
   * @brief Constructs a DequeSet and inserts values from an initializer list in order.
   * @param init_list Values to insert (duplicates ignored).
   */
  DequeSet(std::initializer_list<T> init_list) {
    for (const auto& item : init_list) {
      insert(item);
    }
  }

  // ---- Iteration (in insertion order) ----

  /// @return Iterator to the first element (in insertion order).
  iterator begin() noexcept { return deque_.begin(); }
  /// @return Iterator past the last element.
  iterator end() noexcept { return deque_.end(); }
  /// @return Const iterator to the first element (in insertion order).
  const_iterator begin() const noexcept { return deque_.begin(); }
  /// @return Const iterator past the last element.
  const_iterator end() const noexcept { return deque_.end(); }
  /// @return Const iterator to the first element.
  const_iterator cbegin() const noexcept { return deque_.cbegin(); }
  /// @return Const iterator past the last element.
  const_iterator cend() const noexcept { return deque_.cend(); }

  /**
   * @brief Access the first inserted element.
   * @return Reference to the first element.
   * @pre !empty()
   */
  [[nodiscard]] const T& front() const { return deque_.front(); }

  /**
   * @brief Access the most recently inserted element.
   * @return Reference to the last element.
   * @pre !empty()
   */
  [[nodiscard]] const T& back() const { return deque_.back(); }

  // ---- Lookup ----

  /**
   * @brief Check whether an element exists in the set.
   * @param item Value to look up.
   * @return True if present, false otherwise.
   */
  [[nodiscard]] bool contains(const T& item) const { return set_.contains(item); }

  /**
   * @brief Count occurrences of a value (0 or 1).
   * @param item Value to count.
   * @return 1 if present, otherwise 0.
   */
  [[nodiscard]] size_type count(const T& item) const { return set_.count(item); }

  // ---- Capacity ----

  /**
   * @brief Check whether the container is empty.
   * @return True if empty.
   */
  [[nodiscard]] bool empty() const noexcept { return set_.empty(); }

  /**
   * @brief Number of elements in the container.
   * @return Size of the container.
   */
  [[nodiscard]] size_type size() const noexcept { return set_.size(); }

  /**
   * @brief Reserve buckets in the underlying hash set.
   *
   * Reserving can reduce rehashing overhead during many inserts.
   * This does not reserve space for the deque.
   *
   * @param n Expected number of elements.
   */
  void reserve(size_type n) { set_.reserve(n); }

  // ---- Modifiers ----

  /**
   * @brief Insert a value if it is not already present.
   *
   * @param item Value to insert.
   * @return True if the value was inserted, false if it already existed.
   *
   * @exception Any exception thrown by allocation or copying/moving into the deque.
   * @note Strong exception guarantee: if deque insertion fails, the set insertion is rolled back.
   */
  [[nodiscard]] bool insert(const T& item) {
    auto [it, inserted] = set_.insert(item);
    if (!inserted) {
      return false;
    }

    try {
      deque_.push_back(*it);
    } catch (...) {
      // rollback to keep invariants: set and deque must match
      set_.erase(it);
      throw;
    }
    return true;
  }

  /**
   * @brief Construct and insert a value if it is not already present.
   *
   * @tparam Args Constructor argument types for `T`.
   * @param args Arguments forwarded to construct `T`.
   * @return True if inserted, false if an equivalent value already existed.
   *
   * @exception Any exception thrown by constructing `T`, hashing/equality, allocation, or deque insertion.
   * @note Strong exception guarantee: if deque insertion fails, the set insertion is rolled back.
   */
  template <class... Args>
  [[nodiscard]] bool emplace(Args&&... args) {
    auto [it, inserted] = set_.emplace(std::forward<Args>(args)...);
    if (!inserted) {
      return false;
    }

    try {
      deque_.push_back(*it);
    } catch (...) {
      // rollback to keep invariants: set and deque must match
      set_.erase(it);
      throw;
    }
    return true;
  }

  /**
   * @brief Erase a value if present.
   *
   * @param item Value to erase.
   * @return True if erased, false if not present.
   *
   * @note Complexity is O(n) due to searching the deque for the value.
   * @note The implementation mutates the deque first to reduce the chance of internal desynchronization
   *       if `deque_.erase()` throws for some `T`.
   */
  [[nodiscard]] bool erase(const T& item) {
    auto sit = set_.find(item);
    if (sit == set_.end()) {
      return false;
    }

    const auto eq = set_.key_eq();
    auto dit      = std::find_if(deque_.begin(), deque_.end(), [&](const T& v) { return eq(v, *sit); });

    if (dit == deque_.end()) {
      // Invariant already broken; salvage by removing from set.
      set_.erase(sit);
      return true;
    }

    deque_.erase(dit);
    set_.erase(sit);
    return true;
  }

  /**
   * @brief Remove all elements.
   */
  void clear() noexcept(noexcept(deque_.clear()) && noexcept(set_.clear())) {
    deque_.clear();
    set_.clear();
  }

  // ---- Diagnostics ----

  /**
   * @brief Check internal invariant: both containers have the same elements.
   *
   * @return True if sizes match and every deque element exists in the set.
   * @note Intended for debug/tests; this is O(n).
   */
  [[nodiscard]] bool invariant_holds() const {
    if (deque_.size() != set_.size()) {
      return false;
    }

    for (const auto& v : deque_) {
      if (!set_.contains(v)) {
        return false;
      }
    }

    return true;
  }

 private:
  order_container deque_;
  set_container set_;
};

}  // namespace kEn