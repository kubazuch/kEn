#pragma once
#include <kEn/event/event.hpp>
#include <string>
#include <string_view>

namespace kEn {

class Layer {
 public:
  explicit Layer(std::string_view name = "Layer");
  virtual ~Layer() = default;

  virtual void on_attach() {}
  virtual void on_detach() {}
  virtual void on_update(double /*delta*/, double /*time*/) {}
  virtual void on_render(/*TODO: renderer*/) {}
  virtual void on_imgui() {}
  virtual void on_event(BaseEvent& /*event*/) {}

  const std::string& name() const { return debug_name_; }

  VIRTUAL_FIVE(Layer);

 private:
  std::string debug_name_;
};

}  // namespace kEn
