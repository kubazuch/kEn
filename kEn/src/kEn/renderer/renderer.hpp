#pragma once
#include <mEn.hpp>
#include <mEn/vec3.hpp>

#include <kEn/core/transform.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/vertex_input.hpp>
#include <kEn/scene/camera/camera.hpp>
#include <kEn/scene/light.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

/**
 * @brief High-level scene renderer facade.
 *
 * Static class that manages per-frame camera data, persistent light state, and submission of geometry
 * to the active RenderContext. Expected per-frame call sequence:
 * `begin_scene -> [prepare] -> submit* -> end_scene`.
 */
class Renderer {
 public:
  /**
   * @brief Begin a new render frame.
   *
   * Stores the camera matrices and the RenderContext for this frame. Expected call sequence:
   * `begin_scene -> [prepare] -> submit* -> end_scene`.
   */
  static void begin_scene(const Camera& camera, RenderContext& ctx);
  /** @copydoc begin_scene(const Camera&, RenderContext&) */
  static void begin_scene(const mEn::Vec3& camera_pos, const mEn::Mat4& view, const mEn::Mat4& projection,
                          RenderContext& ctx);

  /**
   * @brief End the current render frame.
   *
   * This is intentionally a no-op. Lights are persistent scene state: they are registered once via
   * add_light() and remain until explicitly removed. Calling end_scene() does NOT clear lights -- it
   * only closes the per-frame submission window opened by begin_scene().
   */
  static void end_scene();

  /**
   * @brief Register a persistent point light.
   *
   * Lights are retained across frames; they are not cleared by end_scene(). Register each light once
   * (e.g. at scene setup), not every frame.
   */
  static void add_light(PointLight& light) { scene_data_->point_lights.push_back(&light); }
  /** @copydoc add_light(PointLight&) */
  static void add_light(DirectionalLight& light) { scene_data_->directional_lights.push_back(&light); }
  /** @copydoc add_light(PointLight&) */
  static void add_light(SpotLight& light) { scene_data_->spot_lights.push_back(&light); }
  /** @brief Set the scene ambient light colour applied to all lit submissions. */
  static void set_ambient(const mEn::Vec3& ambient) { scene_data_->ambient = ambient; }

  /**
   * @brief Upload light and ambient uniforms to a shader.
   *
   * This is an opt-in step required only for lit passes. Shadow passes and other depth-only passes
   * should call submit() directly without calling prepare(). Call once per shader before the first
   * submit() that uses that shader in a lit pass.
   */
  static void prepare(Shader& shader);

  /** @brief Redirect subsequent rendering to @p fb. */
  static void set_render_target(Framebuffer& fb);
  /** @brief Restore the default (window) render target. */
  static void bind_default_render_target();

  /**
   * @brief Submit geometry for rendering without a model transform.
   *
   * Uploads camera uniforms and draws @p vertex_input using @p shader. Must be called inside a
   * begin_scene / end_scene block.
   */
  static void submit(Shader& shader, const VertexInput& vertex_input, RenderMode mode = RenderMode::Triangles);
  /**
   * @brief Submit geometry for rendering with a world-space model transform.
   *
   * Uploads camera uniforms and the `u_M` model matrix from @p transform, then draws @p vertex_input.
   * Must be called inside a begin_scene / end_scene block.
   */
  static void submit(Shader& shader, const VertexInput& vertex_input, const Transform& transform,
                     RenderMode mode = RenderMode::Triangles);
  /**
   * @brief Submit geometry for hardware-instanced rendering.
   *
   * Draws @p instance_count instances of @p vertex_input. Instanced attributes must be configured in
   * the vertex input's buffer layout. Must be called inside a begin_scene / end_scene block.
   */
  static void submit_instanced(Shader& shader, const VertexInput& vertex_input, std::size_t instance_count,
                               RenderMode mode = RenderMode::Triangles);
  /**
   * @brief Submit geometry for tessellated rendering.
   *
   * Configures @p patch_vertex_count control points per patch, uploads camera uniforms and `u_M`,
   * then draws using the Patches primitive topology. Must be called inside a begin_scene / end_scene block.
   */
  static void submit_tessellated(Shader& shader, const VertexInput& vertex_input, std::size_t patch_vertex_count,
                                 const Transform& transform);

 private:
  struct SceneData {
    RenderContext* ctx = nullptr;
    mEn::Mat4 v_matrix;
    mEn::Mat4 p_matrix;
    mEn::Mat4 vp_matrix;
    mEn::Vec3 camera_pos{};
    std::vector<PointLight*> point_lights;
    std::vector<DirectionalLight*> directional_lights;
    std::vector<SpotLight*> spot_lights;
    mEn::Vec3 ambient{};
  };

  static std::unique_ptr<SceneData> scene_data_;
};

}  // namespace kEn
