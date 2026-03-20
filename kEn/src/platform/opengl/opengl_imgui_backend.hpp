#pragma once

#include <kEn/imgui/imgui_backend.hpp>

namespace kEn {

class OpenglImguiBackend final : public ImguiBackend {
 public:
  void init(GLFWwindow* window) override;
  void shutdown() override;
  void new_frame() override;
  void render() override;
};

}  // namespace kEn
