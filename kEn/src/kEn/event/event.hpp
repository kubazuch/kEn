#pragma once

#include <functional>
#include <typeindex>

#define KEN_EVENT_SUBSCRIBER(function) [this](auto& event) { return function(event); }

// TODO: make more automatic using templates
namespace kEn {

struct BaseEvent {
  bool handled = false;

  BaseEvent()          = default;
  virtual ~BaseEvent() = default;

  virtual std::type_index event_id() const = 0;

  VIRTUAL_FIVE(BaseEvent);
};

template <typename EventType>
struct Event : public BaseEvent {
 public:
  std::type_index event_id() const override { return std::type_index(typeid(EventType)); }
};

class EventDispatcher {
 public:
  template <typename EventType>
  using callback_t = std::function<bool(EventType&)>;

  template <typename EventType, typename F>
  void subscribe(const F& callback_fn) {
    const auto id = std::type_index(typeid(EventType));
    subscribers_.emplace(id, CallbackWrapper<EventType>(callback_fn));
  }

  template <typename EventType>
  bool dispatch(EventType& event) {
    const auto id = std::type_index(typeid(EventType));
    for (auto [begin_it, end_it] = subscribers_.equal_range(id); begin_it != end_it; ++begin_it) {
      event.handled |= begin_it->second(event);
      if (event.handled) {
        return true;
      }
    }

    return false;
  }

  bool dispatch(BaseEvent& event) {
    const auto id = event.event_id();
    for (auto [begin_it, end_it] = subscribers_.equal_range(id); begin_it != end_it; ++begin_it) {
      event.handled |= begin_it->second(event);
      if (event.handled) {
        return true;
      }
    }

    return false;
  }

 private:
  std::unordered_multimap<std::type_index, callback_t<BaseEvent>> subscribers_;

  template <typename EventType>
  class CallbackWrapper {
   public:
    template <typename F>
    explicit CallbackWrapper(const F& callback) : callback_(callback) {}

    bool operator()(BaseEvent& event) { return callback_(static_cast<EventType&>(event)); }

   private:
    callback_t<EventType> callback_;
  };
};

}  // namespace kEn
