#pragma once

#include <imgui/imgui.h>

#include <cstdint>

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief Opaque handle to a framebuffer attachment texture.
 *
 * Wraps a platform-native GPU texture identifier (OpenGL texture name,
 * D3D11 ShaderResourceView pointer cast to integer, etc.) returned by
 * @ref Framebuffer::color_attachment and @ref Framebuffer::depth_attachment.
 * Pass directly to @ref RenderContext::bind_attachment or use @ref imgui_id()
 * to hand the texture to ImGui::Image().
 */
struct AttachmentHandle {
  std::uintptr_t value; /**< Platform-native texture identifier. */

  /** @brief Returns a handle suitable for passing to ImGui::Image(). */
  [[nodiscard]] ImTextureID imgui_id() const noexcept { return static_cast<ImTextureID>(value); }
};

}  // namespace kEn
