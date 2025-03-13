#pragma once
#include <kEn/core/transform.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kEn/scene/camera/camera.hpp>
#include <mEn.hpp>

namespace kEn {

class Renderer {
 public:
  static void begin_scene(const std::shared_ptr<Camera>& camera);
  static void end_scene();

  static void add_light(const std::shared_ptr<PointLight>& light) { scene_data_->point_lights.push_back(light); }
  static void add_light(const std::shared_ptr<DirectionalLight>& light) {
    scene_data_->directional_lights.push_back(light);
  }
  static void add_light(const std::shared_ptr<SpotLight>& light) { scene_data_->spot_lights.push_back(light); }
  static void set_ambient(const mEn::Vec3& ambient) { scene_data_->ambient = ambient; }
  static void set_fog(float fog) { scene_data_->fog = fog; }
  static void prepare(Shader& shader);

  static void submit(Shader& shader, const VertexArray& vertex_array);
  static void submit(Shader& shader, const VertexArray& vertex_array, const Transform& transform);
  static void submit_tessellated(Shader& shader, const VertexArray& vertex_array, const uint32_t& count,
                                 const Transform& transform);

 private:
  struct SceneData {
    mEn::Mat4 VP_matrix;
    mEn::Vec3 camera_pos;
    std::vector<std::shared_ptr<PointLight>> point_lights;
    std::vector<std::shared_ptr<DirectionalLight>> directional_lights;
    std::vector<std::shared_ptr<SpotLight>> spot_lights;
    mEn::Vec3 ambient;
    float fog;
  };

  static std::unique_ptr<SceneData> scene_data_;
};

}  // namespace kEn
