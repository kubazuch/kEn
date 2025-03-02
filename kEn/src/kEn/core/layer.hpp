#pragma once
#include <kEn/event/event.hpp>
#include <string>

namespace kEn {

class layer {
 public:
  layer(const std::string& name = "Layer");
  virtual ~layer() = default;

  virtual void on_attach() {}
  virtual void on_detach() {}
  virtual void on_update(double delta, double time) {}
  virtual void on_render(/*TODO: renderer*/) {}
  virtual void on_imgui() {}
  virtual void on_event(base_event& event) {}

  const std::string& name() const { return debug_name_; }

  VIRTUAL_FIVE(layer);

 private:
  std::string debug_name_;
};

}  // namespace kEn
