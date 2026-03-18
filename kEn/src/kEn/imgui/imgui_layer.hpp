#pragma once

#include <kEn/core/layer.hpp>
#include <kEn/event/event.hpp>

namespace kEn {

class ImguiLayer final : public Layer {
 public:
  ImguiLayer();
  ~ImguiLayer() override = default;

  void on_attach() override;
  void on_detach() override;
  void on_imgui() override;
  bool on_event(BaseEvent& event) override;

 private:
  bool on_mouse_event(BaseEvent&);
  bool on_keyboard_event(BaseEvent&);

  EventDispatcher dispatcher_;
};

}  // namespace kEn
