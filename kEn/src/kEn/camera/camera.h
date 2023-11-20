#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "kEn/event/application_events.h"
#include "kEn/core/transform.h"

namespace kEn
{
	class camera
	{
	public:
		virtual ~camera() = default;

		const transform& get_transform() const { return transform_; }

		void set_parent(transform* parent) { transform_.set_parent(parent); recalculate_view(); }
		void set_pos(const glm::vec3& position) { transform_.set_pos(position); recalculate_view(); }
		void set_rot(const glm::quat& rotation) { transform_.set_rot(rotation); recalculate_view(); }
		void rotate(const glm::vec3& axis, float angle) { transform_.rotate(axis, angle); recalculate_view(); }
		void rotate(const glm::quat& rotation) { transform_.rotate(rotation); recalculate_view(); }
		void rotate_local(const glm::quat& rotation) { transform_.rotate_local(rotation); recalculate_view(); }
		void look_at(const glm::vec3& point, const glm::vec3& up) { transform_.look_at(point, up); recalculate_view(); }
		void fma(const glm::vec3& axis, float amount) { transform_.fma(axis, amount); recalculate_view(); }

		const glm::mat4& projection_matrix() const { return projection_matrix_; }
		const glm::mat4& view_matrix() const { return view_matrix_; }
		const glm::mat4& view_projection_matrix() const { return view_projection_matrix_; }

		virtual bool on_window_resize(window_resize_event& event) = 0;

		void recalculate_view();

	protected:
		glm::mat4 projection_matrix_;
		glm::mat4 view_matrix_;
		glm::mat4 view_projection_matrix_;

		transform transform_;
	};

	class orthographic_camera : public camera
	{
	public:
		orthographic_camera(float left, float right, float bottom, float top);

		void set_projection(float left, float right, float bottom, float top);

		bool on_window_resize(window_resize_event& event) override;
	private:
		float left_, right_, bottom_, top_;
	};

	class perspective_camera : public camera
	{
	public:
		perspective_camera(float fov, float aspect, float zNear, float zFar);

		void set_projection(float fov, float aspect, float zNear, float zFar);

		bool on_window_resize(window_resize_event& event) override;
	private:
		float fov_, aspect_, zNear_, zFar_;
	};
}
