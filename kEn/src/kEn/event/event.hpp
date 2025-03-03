#pragma once

#include <functional>
#include <kEn/core/core.hpp>
#include <string>

#define KEN_EVENT_SUBSCRIBER(function) [this](auto& event) { return function(event); }

namespace kEn {

class BaseEvent {
 public:
  bool handled = false;

  BaseEvent()          = default;
  virtual ~BaseEvent() = default;

  virtual const char* name() const = 0;
  virtual std::string to_string() const { return name(); }
  virtual std::size_t event_id() = 0;

  VIRTUAL_FIVE(BaseEvent);

 protected:
  static std::size_t next_id() {
    static std::size_t id = 0;
    return id++;
  }
};

template <typename EventType>
class Event : public BaseEvent {
 public:
  std::size_t event_id() override { return id(); }

  static std::size_t id() {
    static auto id = BaseEvent::next_id();
    return id;
  }
};

class EventDispatcher {
 public:
  template <typename EventType>
  using callback_t = std::function<bool(EventType&)>;

  template <typename EventType, typename F>
  void subscribe(const F& callback_fn) {
    const auto id = EventType::id();
    subscribers_[id].push_back(CallbackWrapper<EventType>(callback_fn));
  }

  template <typename EventType>
  bool dispatch(EventType& event) {
    const auto id = EventType::id();
    for (auto& callback : subscribers_[id]) {
      event.handled |= callback(event);
      if (event.handled) {
        return true;
      }
    }

    return false;
  }

  bool dispatch(BaseEvent& event) {
    const auto id = event.event_id();
    for (auto& callback : subscribers_[id]) {
      event.handled |= callback(event);
      if (event.handled) {
        return true;
      }
    }

    return false;
  }

 private:
  std::unordered_map<std::size_t, std::vector<callback_t<BaseEvent>>> subscribers_;

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

inline std::ostream& operator<<(std::ostream& os, const BaseEvent& e) { return os << e.to_string(); }

}  // namespace kEn
