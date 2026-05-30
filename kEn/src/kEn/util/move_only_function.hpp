#pragma once

#include <functional>
#include <version>

#include <kEn/core/core.hpp>

#if defined(__cpp_lib_move_only_function) && __cpp_lib_move_only_function >= 202110L

namespace kEn {

template <class Sig>
using move_only_function = std::move_only_function<Sig>;

}  // namespace kEn

#else

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

namespace kEn {

template <class Sig>
class move_only_function;

/**
 * @brief Move-only type-erased callable wrapper.
 *
 * Drop-in replacement for C++23 @c std::move_only_function for toolchains that
 * do not yet implement it (e.g. libc++ as of LLVM 20).  Stores the callable
 * behind a @c unique_ptr, so the callable itself need not be copyable.
 *
 * @tparam R    Return type.
 * @tparam Args Argument types.
 */
template <class R, class... Args>
class move_only_function<R(Args...)> {
  struct Concept {
    virtual ~Concept()      = default;
    virtual R call(Args...) = 0;
  };

  template <class F>
  struct Model final : Concept {
    F fn;
    template <class Fn>
    explicit Model(Fn&& f) : fn(std::forward<Fn>(f)) {}
    R call(Args... args) override {
      if constexpr (std::is_void_v<R>) {
        std::invoke(fn, std::forward<Args>(args)...);
      } else {
        return std::invoke(fn, std::forward<Args>(args)...);
      }
    }
  };

 public:
  move_only_function() noexcept = default;
  move_only_function(std::nullptr_t) noexcept {}  // NOLINT(google-explicit-constructor)

  move_only_function(move_only_function&&) noexcept            = default;
  move_only_function& operator=(move_only_function&&) noexcept = default;

  move_only_function(const move_only_function&)            = delete;
  move_only_function& operator=(const move_only_function&) = delete;

  template <class F>
    requires(!std::is_same_v<std::remove_cvref_t<F>, move_only_function> &&
             std::is_constructible_v<std::decay_t<F>, F> && std::is_invocable_r_v<R, std::decay_t<F>&, Args...>)
  move_only_function(F&& f)  // NOLINT(google-explicit-constructor)
      : target_(std::make_unique<Model<std::decay_t<F> > >(std::forward<F>(f))) {}

  template <class F>
    requires(!std::is_same_v<std::remove_cvref_t<F>, move_only_function> &&
             std::is_constructible_v<std::decay_t<F>, F> && std::is_invocable_r_v<R, std::decay_t<F>&, Args...>)
  move_only_function& operator=(F&& f) {
    target_ = std::make_unique<Model<std::decay_t<F> > >(std::forward<F>(f));
    return *this;
  }

  move_only_function& operator=(std::nullptr_t) noexcept {
    target_.reset();
    return *this;
  }

  explicit operator bool() const noexcept { return static_cast<bool>(target_); }

  R operator()(Args... args) {
    assert(target_ &&
           "calling empty kEn::move_only_function");  // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    if constexpr (std::is_void_v<R>) {
      target_->call(std::forward<Args>(args)...);
    } else {
      return target_->call(std::forward<Args>(args)...);
    }
  }

  friend bool operator==(const move_only_function& fn, std::nullptr_t) noexcept { return !fn; }
  friend bool operator==(std::nullptr_t, const move_only_function& fn) noexcept { return !fn; }

 private:
  std::unique_ptr<Concept> target_;
};

}  // namespace kEn

#endif
