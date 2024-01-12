#pragma once

#include <list>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "kEn/lang/lazy_variable.h"

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
		glm::mat4& local_to_world_matrix() const;
		glm::mat4 world_to_local_matrix() const;
		void model_matrix_updated();

		void rotate(const glm::vec3& axis, float angle);
		void rotate(const glm::quat& rotation);
		void rotate_local(const glm::quat& rotation);
		void look_at(const glm::vec3& point, const glm::vec3& up);
		void fma(const glm::vec3& axis, float amount);

		//glm::vec3 pos() const { return pos_; }
		//glm::vec3& pos() { return pos_.get(); }
		const lazy_variable<glm::vec3>& pos() const { return pos_; }
		lazy_variable<glm::vec3>& pos() { return pos_; }
		glm::vec3 transformed_pos() const { return parent_ ? glm::vec3(parent_->local_to_world_matrix() * glm::vec4(pos_.get(), 1.f)): pos_; }
		void set_pos(const glm::vec3& pos) { pos_ = pos; set_dirty(); }

		// glm::quat rot() const { return rot_; }
		const lazy_variable<glm::quat>& rot() const { return rot_; }
		lazy_variable<glm::quat>& rot() { return rot_; }
		glm::quat transformed_rot() const { return parent_ ? parent_->transformed_rot() * rot_.get() : rot_; }
		void set_rot(const glm::quat& rot) { rot_ = rot; set_dirty(); }

		//glm::vec3 scale() const { return scale_; }
		const lazy_variable<glm::vec3>& scale() const { return scale_; }
		lazy_variable<glm::vec3>& scale() { return scale_; }
		void set_scale(const glm::vec3& scale) { scale_ = scale; set_dirty(); }

		glm::vec3 local_front() const;
		glm::vec3 local_right() const;
		glm::vec3 local_up() const;

		glm::vec3 front() const;
		glm::vec3 right() const;
		glm::vec3 up() const;

		transform(const transform&) = delete;
		transform(transform&&) = delete;
		transform& operator=(const transform&) noexcept = delete;
		transform& operator=(transform&&) noexcept = delete;
	private:
		void set_dirty();

	private:
		transform* parent_;
		std::list<transform*> children_;

		lazy_variable<glm::vec3> pos_;
		lazy_variable<glm::quat> rot_;
		lazy_variable<glm::vec3> scale_;

		mutable bool dirty_ = false;
		mutable glm::mat4 model_mat_ = glm::mat4(1);

		mutable bool inverse_dirty_ = false;
		mutable glm::mat4 inv_model_mat_ = glm::mat4(1);
	};
	
}
