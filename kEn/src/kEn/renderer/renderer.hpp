#pragma once
#include <kEn/core/transform.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kEn/scene/camera/camera.hpp>

namespace kEn {

class renderer {
 public:
  static void begin_scene(std::shared_ptr<camera> camera);
  static void end_scene();

  static void add_light(const std::shared_ptr<point_light>& light) { scene_data_->point_lights.push_back(light); }
  static void add_light(const std::shared_ptr<directional_light>& light) {
    scene_data_->directional_lights.push_back(light);
  }
  static void add_light(const std::shared_ptr<spot_light>& light) { scene_data_->spot_lights.push_back(light); }
  static void set_ambient(const glm::vec3& ambient) { scene_data_->ambient = ambient; }
  static void set_fog(float fog) { scene_data_->fog = fog; }
  static void prepare(shader& shader);

  static void submit(shader& shader, const vertex_array& vertex_array, const transform& transform);
  static void submit_tessellated(shader& shader, const vertex_array& vertex_array, const uint32_t& count,
                                 const transform& transform);

 private:
  struct scene_data {
    glm::mat4 VP_matrix;
    glm::vec3 camera_pos;
    std::vector<std::shared_ptr<point_light>> point_lights;
    std::vector<std::shared_ptr<directional_light>> directional_lights;
    std::vector<std::shared_ptr<spot_light>> spot_lights;
    glm::vec3 ambient;
    float fog;
  };

  static std::unique_ptr<scene_data> scene_data_;
};

}  // namespace kEn
