#pragma once
#include <mEn.hpp>
#include <mEn/vec3.hpp>

#include <kEn/core/transform.hpp>
#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/vertex_input.hpp>
#include <kEn/scene/camera/camera.hpp>
#include <kEn/scene/light.hpp>

namespace kEn {

class Renderer {
 public:
  static void begin_scene(const Camera& camera);
  static void begin_scene(const mEn::Vec3& camera_pos, const mEn::Mat4& view, const mEn::Mat4& projection);
  static void end_scene();

  static void add_light(std::shared_ptr<PointLight> light) { scene_data_->point_lights.push_back(std::move(light)); }
  static void add_light(std::shared_ptr<DirectionalLight> light) {
    scene_data_->directional_lights.push_back(std::move(light));
  }
  static void add_light(std::shared_ptr<SpotLight> light) { scene_data_->spot_lights.push_back(std::move(light)); }
  static void set_ambient(const mEn::Vec3& ambient) { scene_data_->ambient = ambient; }
  static void set_fog(float fog) { scene_data_->fog = fog; }
  static void prepare(Shader& shader);

  static void submit(Shader& shader, const VertexInput& vertex_input, RenderMode mode = RenderMode::Triangles);
  static void submit(Shader& shader, const VertexInput& vertex_input, const Transform& transform,
                     RenderMode mode = RenderMode::Triangles);
  static void submit_instanced(Shader& shader, const VertexInput& vertex_input, std::size_t instance_count,
                               RenderMode mode = RenderMode::Triangles);
  static void submit_tessellated(Shader& shader, const VertexInput& vertex_input, std::size_t count,
                                 const Transform& transform);

 private:
  struct SceneData {
    mEn::Mat4 V_matrix;
    mEn::Mat4 P_matrix;
    mEn::Mat4 VP_matrix;
    mEn::Vec3 camera_pos{};
    std::vector<std::shared_ptr<PointLight>> point_lights;
    std::vector<std::shared_ptr<DirectionalLight>> directional_lights;
    std::vector<std::shared_ptr<SpotLight>> spot_lights;
    mEn::Vec3 ambient{};
    float fog{};
  };

  static std::unique_ptr<SceneData> scene_data_;
};

}  // namespace kEn
