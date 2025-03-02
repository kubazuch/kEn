#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <optional>
#include <vector>

namespace kEn {

class transform {
 public:
  transform(glm::vec3 pos = glm::vec3(), glm::quat rot = {1, 0, 0, 0}, glm::vec3 scale = {1, 1, 1});
  ~transform();

  void set_parent(transform& parent);
  void unset_parent();
  const transform& get_parent() const { return *parent_; }

  glm::mat4 local_to_parent_matrix() const;
  glm::mat4& local_to_world_matrix() const;
  glm::mat4 world_to_local_matrix() const;
  void model_matrix_updated();

  void rotate(const glm::vec3& axis, float angle);
  void rotate(const glm::quat& rotation);
  void rotate_local(const glm::quat& rotation);
  void look_at(const glm::vec3& point, const glm::vec3& up = glm::vec3(0, 1, 0));
  void fma(const glm::vec3& axis, float amount);

  const glm::vec3& local_pos() const { return pos_; }
  glm::vec3& local_pos() { return pos_; }
  glm::vec3 pos() const {
    return parent_.has_value() ? glm::vec3(parent_.value().get().local_to_world_matrix() * glm::vec4(pos_, 1.f)) : pos_;
  }
  void set_local_pos(const glm::vec3& pos) {
    pos_ = pos;
    set_dirty();
  }

  const glm::quat& local_rot() const { return rot_; }
  glm::quat& local_rot() { return rot_; }
  glm::quat rot() const { return parent_.has_value() ? parent_.value().get().rot() * rot_ : rot_; }
  void set_local_rot(const glm::quat& rot) {
    rot_ = rot;
    set_dirty();
  }

  const glm::vec3& local_scale() const { return scale_; }
  glm::vec3& local_scale() { return scale_; }
  void set_local_scale(const glm::vec3& scale) {
    scale_ = scale;
    set_dirty();
  }
  void set_local_scale(float scale) {
    scale_ = glm::vec3(scale);
    set_dirty();
  }

  glm::vec3 local_front() const;
  glm::vec3 local_right() const;
  glm::vec3 local_up() const;

  glm::vec3 front() const;
  glm::vec3 right() const;
  glm::vec3 up() const;

  void set_dirty();

  transform(const transform&)                     = delete;
  transform(transform&&)                          = delete;
  transform& operator=(const transform&) noexcept = delete;
  transform& operator=(transform&&) noexcept      = delete;

 private:
  std::optional<std::reference_wrapper<transform>> parent_;
  std::vector<std::reference_wrapper<transform>> children_;

  glm::vec3 pos_;
  glm::quat rot_;
  glm::vec3 scale_;

  mutable bool dirty_          = false;
  mutable glm::mat4 model_mat_ = glm::mat4(1);

  mutable bool inverse_dirty_      = false;
  mutable glm::mat4 inv_model_mat_ = glm::mat4(1);
};

}  // namespace kEn
