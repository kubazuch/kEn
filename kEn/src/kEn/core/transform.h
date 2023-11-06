#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

namespace kEn
{
	class transform
	{
	public:
		transform();
		~transform();

		void set_parent(transform* parent);
		const transform& get_parent() const { return *parent_; }

		glm::mat4 local_to_parent_matrix() const;
		glm::mat4 local_to_world_matrix() const;
		glm::mat4 world_to_local_matrix() const;

		void rotate(const glm::vec3& axis, float angle);
		void rotate(const glm::quat& rotation);
		void look_at(const glm::vec3& point, const glm::vec3& up);

		glm::vec3 pos() const { return pos_; }
		glm::vec3 transformed_pos() const { return parent_ ? glm::vec3(parent_->local_to_world_matrix() * glm::vec4(pos_, 1.f)): pos_; }
		void set_pos(const glm::vec3& pos) { pos_ = pos; set_dirty(); }

		glm::quat rot() const { return rot_; }
		glm::quat transformed_rot() const { return parent_ ? parent_->transformed_rot() * rot_ : rot_; }
		void set_rot(const glm::quat& rot) { rot_ = rot; set_dirty(); }

		glm::vec3 scale() const { return scale_; }
		void set_scale(const glm::vec3& scale) { scale_ = scale; set_dirty(); }

		transform(const transform&) = delete;
		transform(transform&&) = delete;
		transform& operator=(const transform&) noexcept = delete;
		transform& operator=(transform&&) noexcept = delete;
	private:
		void set_dirty();

	private:
		transform* parent_;
		std::list<transform*> children_;

		glm::vec3 pos_;
		glm::quat rot_;
		glm::vec3 scale_;

		mutable bool dirty_ = false;
		mutable glm::mat4 model_mat_ = glm::mat4(1);

		mutable bool inverse_dirty_ = false;
		mutable glm::mat4 inv_model_mat_ = glm::mat4(1);
	};
	
}