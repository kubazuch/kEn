#pragma once

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

struct GLFWwindow;

namespace kEn {

/**
 * @brief Abstract interface for the Dear ImGui renderer backend.
 *
 * Owns the renderer-specific ImGui backend lifecycle (init, shutdown, per-frame
 * new_frame / render). GLFW platform backend calls (ImGui_ImplGlfw_*) are the
 * responsibility of each concrete implementation since they are tied to the
 * renderer init path (e.g. `ImGui_ImplOpenGL3_Init()`).
 *
 * Obtain the active instance via ImguiBackend::instance() after ImguiLayer
 * has attached. ImguiLayer owns the lifetime of the backend object and registers
 * it as the active instance on attach / clears it on detach.
 */
class ImguiBackend {
 public:
  virtual ~ImguiBackend() = default;

  /** @brief Initializes the platform and renderer ImGui backends. */
  virtual void init(GLFWwindow* window) = 0;
  /** @brief Shuts down the platform and renderer ImGui backends. */
  virtual void shutdown() = 0;
  /** @brief Calls the backend New Frame functions before ImGui::NewFrame(). */
  virtual void new_frame() = 0;
  /** @brief Submits accumulated draw data and handles multi-viewport rendering. */
  virtual void render() = 0;

  /** @brief Returns the active backend. Asserts if none is registered. */
  [[nodiscard]] static ImguiBackend& instance();

  /** @brief Called by ImguiLayer on attach/detach to register or clear the active instance. */
  static void set_instance(ImguiBackend* backend);

 private:
  static ImguiBackend* instance_;
};

}  // namespace kEn
