#pragma once
#include <kEn/core/layer.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class ImguiLayer : public Layer {
 public:
  ImguiLayer();
  ~ImguiLayer() override = default;

  void on_attach() override;
  void on_detach() override;
  void on_update(duration_t, duration_t) override {}
  void on_render(double) override {}
  void on_imgui() override;
  void on_event(BaseEvent& event) override;

  void begin();
  void end();

 private:
  bool on_mouse_event(BaseEvent&);
  bool on_keyboard_event(BaseEvent&);

  EventDispatcher dispatcher_;
};

}  // namespace kEn
