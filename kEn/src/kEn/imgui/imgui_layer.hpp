#pragma once
#include <kEn/core/layer.hpp>

namespace kEn {

class ImguiLayer : public Layer {
 public:
  ImguiLayer();
  ~ImguiLayer() override = default;

  void on_attach() override;
  void on_detach() override;
  void on_update(double, double) override {}
  void on_render() override {}
  void on_imgui() override;
  void on_event(BaseEvent& event) override;

  void begin();
  void end();

 private:
};

}  // namespace kEn
