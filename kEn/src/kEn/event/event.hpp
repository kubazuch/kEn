#pragma once

#include <concepts>
#include <format>
#include <functional>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

/**
 * @def KEN_EVENT_SUBSCRIBER(function)
 * @brief Convenience wrapper to adapt a member-call style function into a subscriber lambda.
 *
 * Expands to a lambda that captures @c this and forwards an event to @p function:
 * @code
 * dispatcher.subscribe<MyEvent>(KEN_EVENT_SUBSCRIBER(on_my_event));
 * @endcode
 *
 * @param function A callable expression that can be invoked as @c function(event).
 *
 * @deprecated Use @ref KEN_BIND_EVENT_HANDLER instead. This macro is kept only for backward
 *             compatibility and may be removed in a future release.
 *
 * @note This macro assumes it is used in a context where @c this is valid (i.e., inside a non-static
 *       member function).
 * @see KEN_BIND_EVENT_HANDLER
 */
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define KEN_EVENT_SUBSCRIBER(function) [this](auto& event) { return function(event); }

/**
 * @def KEN_BIND_EVENT_HANDLER(member_fn)
 * @brief Convenience wrapper to bind a member function as an invocable object.
 *
 * Produces a callable equivalent to:
 * @code
 * std::bind_front(&Class::member_fn, this)
 * @endcode
 *
 * @param member_fn The unqualified member function name (no @c & or class qualification).
 *
 * @note This macro uses @c std::bind_front and assumes it is invoked in a context where @c this is valid.
 */
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define KEN_BIND_EVENT_HANDLER(member_fn) std::bind_front(&std::remove_reference_t<decltype(*this)>::member_fn, this)

namespace kEn {  // NOLINT

namespace detail {

/**
 * @brief Extract basic signature traits from simple callables.
 *
 * The dispatcher uses these traits to deduce the first argument type of a callable so that
 * @ref kEn::EventDispatcher::subscribe(F&&) can infer the event type it should subscribe to.
 *
 * Supported forms include:
 * - Free function pointers: @c R(*)(Arg) (including noexcept variants)
 * - Member function pointers: @c R(C::*)(Arg) (including const/noexcept variants)
 * - Functors/lambdas with an accessible @c operator()
 *
 * @tparam T Callable type (or function pointer/member pointer type).
 */
template <class T>
struct callable_traits;

template <class R, class Arg>
struct callable_traits<R (*)(Arg)> {
  using result_t = R;
  using arg1_t   = Arg;
};

template <class R, class Arg>
struct callable_traits<R (*)(Arg) noexcept> {
  using result_t = R;
  using arg1_t   = Arg;
};

template <class R, class C, class Arg>
struct callable_traits<R (C::*)(Arg)> {
  using result_t = R;
  using arg1_t   = Arg;
  using class_t  = C;
};

template <class R, class C, class Arg>
struct callable_traits<R (C::*)(Arg) const> {
  using result_t = R;
  using arg1_t   = Arg;
  using class_t  = C;
};

template <class R, class C, class Arg>
struct callable_traits<R (C::*)(Arg) noexcept> {
  using result_t = R;
  using arg1_t   = Arg;
  using class_t  = C;
};

template <class R, class C, class Arg>
struct callable_traits<R (C::*)(Arg) const noexcept> {
  using result_t = R;
  using arg1_t   = Arg;
  using class_t  = C;
};

template <class F>
  requires requires { &std::remove_reference_t<F>::operator(); }
struct callable_traits<F> : callable_traits<decltype(&std::remove_reference_t<F>::operator())> {};

/**
 * @brief Concept satisfied by callables whose first argument and return type can be deduced.
 *
 * @tparam F Callable type.
 */
template <class F>
concept deducible_single_arg_callable = requires {
  typename callable_traits<std::decay_t<F>>::arg1_t;
  typename callable_traits<std::decay_t<F>>::result_t;
};

/**
 * @brief First argument type for a deducible callable.
 * @tparam F Callable type.
 */
template <class F>
using callable_arg1_t = typename callable_traits<std::decay_t<F>>::arg1_t;

/**
 * @brief Result type for a deducible callable.
 * @tparam F Callable type.
 */
template <class F>
using callable_result_t = typename callable_traits<std::decay_t<F>>::result_t;

}  // namespace detail

/**
 * @brief Abstract base type for all events.
 *
 * Events are dispatched via @ref kEn::EventDispatcher. Each event provides:
 * - a stable runtime type identifier via @ref event_id()
 * - a textual representation written to a stream via @ref write()
 *
 * The @ref handled flag is used by the dispatcher to stop propagation early.
 */
struct BaseEvent {
  /**
   * @brief Whether this event has been handled.
   *
   * The dispatcher ORs this value with subscriber return values and stops dispatching once
   * it becomes true.
   */
  bool handled{false};

  /** @brief Default constructor. */
  constexpr BaseEvent() noexcept = default;
  /** @brief Virtual destructor for polymorphic use. */
  virtual ~BaseEvent() = default;

  /**
   * @brief Return the dynamic event type identifier.
   * @return A type_index identifying the runtime type of the event.
   */
  virtual std::type_index event_id() const noexcept = 0;

  /**
   * @brief Write a human-readable representation of the event.
   * @param os Output stream to write to.
   */
  virtual void write(std::ostream& os) const = 0;

  /**
   * @brief Convert the event to a string by streaming @ref write().
   * @return The string representation produced by @ref write().
   */
  [[nodiscard]] std::string to_string() const {
    std::ostringstream oss;
    write(oss);
    return oss.str();
  }

  VIRTUAL_FIVE(BaseEvent);
};

/**
 * @brief Stream insertion for events.
 * @param os Output stream.
 * @param e Event to write.
 * @return @p os.
 */
inline std::ostream& operator<<(std::ostream& os, const BaseEvent& e) {
  e.write(os);
  return os;
}

/**
 * @brief Customization for fmt::format that formats events via @ref BaseEvent::to_string().
 *
 * @tparam T Any type derived from @ref kEn::BaseEvent.
 * @param e Event instance.
 * @return String form of @p e.
 *
 * @note This participates in spdlog customization.
 */
template <class T>
  requires std::derived_from<std::remove_cvref_t<T>, BaseEvent>
inline std::string format_as(const T& e) {
  return e.to_string();
}

/**
 * @brief Convenience base class for concrete event types.
 *
 * Inherit your event type from @c Event<YourType> to get:
 * - a stable type identifier via @ref static_id()
 * - a default name via @ref static_name()
 * - default implementations of @ref BaseEvent::event_id() and @ref BaseEvent::write()
 *
 * @tparam EventType The concrete event type (usually the derived type).
 *
 * @note Naming preference order for @ref static_name():
 *       1) @c EventType::kName (a static string_view-like constant)
 *       2) @c EventType::static_name()
 *       3) @c typeid(EventType).name() (may be compiler-mangled)
 */
template <typename EventType>
struct Event : public BaseEvent {
  /**
   * @brief Static type identifier for @p EventType.
   * @return Reference to a process-stable @c std::type_index for @p EventType.
   */
  static const std::type_index& static_id() noexcept {
    static const std::type_index kId{typeid(EventType)};
    return kId;
  }

  /**
   * @brief Static display name for @p EventType.
   * @return A string view naming the event type.
   */
  static constexpr std::string_view static_name() noexcept {
    if constexpr (requires { EventType::kName; }) {
      return EventType::kName;
    } else if constexpr (requires { EventType::static_name(); }) {
      return EventType::static_name();
    } else {
      // Fallback: may be compiler-mangled
      return typeid(EventType).name();
    }
  }

  /** @brief @copydoc kEn::BaseEvent::event_id */
  std::type_index event_id() const noexcept override { return static_id(); }

  /**
   * @brief Default stream output: writes @ref static_name().
   * @param os Output stream.
   */
  void write(std::ostream& os) const override { os << static_name(); }
};

/**
 * @brief Minimal event dispatcher with type-based subscription.
 *
 * Subscribers are stored in a type-erased callable wrapper:
 * @ref base_callback_t, which takes a @ref kEn::BaseEvent& and returns @c bool.
 *
 * Dispatch behavior:
 * - First, invoke subscribers registered for the event's dynamic type.
 * - If none handled it and the event type isn't already @ref kEn::BaseEvent, invoke
 *   "catch-all" subscribers registered for @ref kEn::BaseEvent.
 * - If any subscriber returns true, the event's @ref kEn::BaseEvent::handled flag is set
 *   and dispatch stops early.
 *
 * @note Subscriber iteration order depends on the underlying container and is not guaranteed.
 * @note This class is not inherently thread-safe; external synchronization is required if used
 *       concurrently.
 */
class EventDispatcher {
 public:
  /** @brief Construct an empty dispatcher. */
  EventDispatcher() = default;
  /** @brief Destructor. */
  ~EventDispatcher() = default;

  /** @brief Non-copyable (subscribers are move-only). */
  EventDispatcher(const EventDispatcher&) = delete;
  /** @brief Non-copyable (subscribers are move-only). */
  EventDispatcher& operator=(const EventDispatcher&) = delete;
  /** @brief Movable. */
  EventDispatcher(EventDispatcher&&) noexcept = default;
  /** @brief Movable. */
  EventDispatcher& operator=(EventDispatcher&&) noexcept = default;

 private:
  /**
   * @brief Constraint for a subscriber callable for a specific event type.
   *
   * @tparam F Callable type.
   * @tparam E Event type.
   */
  template <class F, class E>
  static constexpr bool subscriber_for_v =  // NOLINT(readability-identifier-naming)
      std::derived_from<E, BaseEvent> && std::invocable<F, E&> &&
      std::convertible_to<std::invoke_result_t<F, E&>, bool>;

 public:
  /**
   * @brief Type-erased subscriber callable type.
   *
   * The callable receives a @ref kEn::BaseEvent& and returns @c true if the event is handled.
   */
  using base_callback_t = std::move_only_function<bool(BaseEvent&)>;

  /**
   * @brief Subscribe a callable to a specific event type.
   *
   * @tparam EventType Event type to subscribe to.
   * @tparam F Callable type. Must be invocable with @c EventType& and return something convertible to @c bool.
   * @param callback_fn Subscriber callable.
   *
   * @note The callable is wrapped and stored type-erased as a @ref base_callback_t.
   */
  template <typename EventType, typename F>
    requires subscriber_for_v<F, EventType>
  void subscribe(F&& callback_fn) {
    auto wrapper = [fn = std::forward<F>(callback_fn)](BaseEvent& event) mutable -> bool {
      return static_cast<bool>(std::invoke(fn, static_cast<EventType&>(event)));
    };

    subscribers_.emplace(Event<EventType>::static_id(), base_callback_t{std::move(wrapper)});
  }

  /**
   * @brief Subscribe a callable and deduce the event type from its first argument.
   *
   * If @p callback_fn is a callable whose first argument type is an event type derived
   * from @ref kEn::BaseEvent, this overload deduces the event type and subscribes it.
   *
   * @tparam F Callable type.
   * @param callback_fn Subscriber callable.
   */
  template <typename F>
    requires detail::deducible_single_arg_callable<F> &&
             subscriber_for_v<F, std::remove_cvref_t<detail::callable_arg1_t<F>>>
  void subscribe(F&& callback_fn) {
    using EventType = std::remove_cvref_t<detail::callable_arg1_t<F>>;
    this->template subscribe<EventType>(std::forward<F>(callback_fn));
  }

  /**
   * @brief Subscribe a member function on an object reference.
   *
   * @tparam Obj Object type.
   * @tparam MemFn Member function pointer type.
   * @param obj Target object.
   * @param memfn Member function pointer to invoke as @c std::invoke(memfn, obj, event).
   *
   * @note The member function must accept exactly one event argument and return something
   *       convertible to @c bool.
   */
  template <class Obj, class MemFn>
    requires std::is_member_function_pointer_v<std::decay_t<MemFn>> && detail::deducible_single_arg_callable<MemFn> &&
             std::derived_from<std::remove_cvref_t<detail::callable_arg1_t<MemFn>>, BaseEvent> &&
             std::invocable<MemFn, Obj&, std::remove_cvref_t<detail::callable_arg1_t<MemFn>>&> &&
             std::convertible_to<
                 std::invoke_result_t<MemFn, Obj&, std::remove_cvref_t<detail::callable_arg1_t<MemFn>>&>, bool>
  void subscribe(Obj& obj, MemFn memfn) {
    using EventType = std::remove_cvref_t<detail::callable_arg1_t<MemFn>>;
    this->template subscribe<EventType>(
        [&obj, memfn](EventType& e) -> bool { return static_cast<bool>(std::invoke(memfn, obj, e)); });
  }

  /**
   * @brief Subscribe a member function on an object pointer.
   *
   * @tparam Obj Object type.
   * @tparam MemFn Member function pointer type.
   * @param obj Target object pointer (must be non-null).
   * @param memfn Member function pointer.
   */
  template <class Obj, class MemFn>
    requires std::is_member_function_pointer_v<std::decay_t<MemFn>>
  void subscribe(Obj* obj, MemFn memfn) {
    subscribe(*obj, memfn);
  }

  /**
   * @brief Dispatch a concrete event type.
   *
   * @tparam EventType Event type.
   * @param event Event instance.
   * @return True if the event became handled.
   */
  template <typename EventType>
    requires std::derived_from<EventType, BaseEvent>
  [[nodiscard]] bool dispatch(EventType& event) {
    return dispatch(static_cast<BaseEvent&>(event));
  }

  /**
   * @brief Dispatch an event through the subscription table.
   *
   * The dispatcher first tries subscribers matching the event's dynamic type.
   * If none handle it, it then tries catch-all subscribers registered under
   * @ref kEn::BaseEvent (unless the event is already BaseEvent).
   *
   * @param event Event instance (polymorphic).
   * @return True if the event became handled.
   */
  [[nodiscard]] bool dispatch(BaseEvent& event) {
    // Exact match subscribers for the event's dynamic type.
    const auto id = event.event_id();
    if (dispatch_by_id(id, event)) {
      return true;
    }

    // Catch-all subscribers: subscribe<BaseEvent>(...) receives every event.
    if (id != Event<BaseEvent>::static_id()) {
      return dispatch_by_id(Event<BaseEvent>::static_id(), event);
    }
    return false;
  }

  /** @brief Remove all subscribers. */
  void clear() noexcept { subscribers_.clear(); }

  /** @brief @return True if there are no subscribers. */
  [[nodiscard]] bool empty() const noexcept { return subscribers_.empty(); }
  /** @brief @return Number of stored subscribers (across all event types). */
  [[nodiscard]] std::size_t size() const noexcept { return subscribers_.size(); }

 private:
  /**
   * @brief Subscriber storage keyed by event type identifier.
   *
   * Multiple subscribers may be registered for the same event type.
   */
  std::unordered_multimap<std::type_index, base_callback_t> subscribers_;

  /**
   * @brief Dispatch to subscribers registered under a given type id.
   * @param id Event type id key.
   * @param event Event to pass to subscribers.
   * @return True if the event became handled.
   */
  [[nodiscard]] bool dispatch_by_id(const std::type_index& id, BaseEvent& event) {
    for (auto [it, end] = subscribers_.equal_range(id); it != end; ++it) {
      event.handled |= it->second(event);
      if (event.handled) {
        return true;
      }
    }
    return false;
  }
};

}  // namespace kEn

namespace std {

/**
 * @brief std::format support for @ref kEn::BaseEvent.
 *
 * Formats the event by calling @ref kEn::BaseEvent::to_string() and then formatting
 * the resulting string using the underlying @c formatter<string_view>.
 */
template <>
struct formatter<kEn::BaseEvent, char> : formatter<std::string_view, char> {
  /** @brief Parse format specifiers as for @c string_view. */
  constexpr auto parse(format_parse_context& ctx) { return formatter<std::string_view, char>::parse(ctx); }

  /**
   * @brief Format the event.
   * @param e Event to format.
   * @param ctx Format context.
   * @return Iterator into the output buffer.
   */
  template <class FormatContext>
  auto format(const kEn::BaseEvent& e, FormatContext& ctx) const {
    const std::string s = e.to_string();
    return formatter<std::string_view, char>::format(std::string_view{s}, ctx);
  }
};

/**
 * @brief std::format support for any type derived from @ref kEn::BaseEvent.
 *
 * Delegates to @ref std::formatter<kEn::BaseEvent, char> by casting the derived event
 * to its base reference.
 *
 * @tparam T Derived event type.
 */
template <class T>
  requires(std::derived_from<T, kEn::BaseEvent> && !std::same_as<T, kEn::BaseEvent>)
struct formatter<T, char> : formatter<kEn::BaseEvent, char> {
  /**
   * @brief Format the derived event via the BaseEvent formatter.
   * @param e Derived event to format.
   * @param ctx Format context.
   * @return Iterator into the output buffer.
   */
  template <class FormatContext>
  auto format(const T& e, FormatContext& ctx) const {
    return formatter<kEn::BaseEvent, char>::format(static_cast<const kEn::BaseEvent&>(e), ctx);
  }
};

}  // namespace std
