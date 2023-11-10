#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "kEn/event/application_events.h"

namespace kEn
{
	class camera
	{
	public:
		virtual ~camera() = default;

		const glm::vec3& position() const { return position_; }
		void set_position(const glm::vec3& position) { position_ = position; recalculate_view(); }

		const glm::quat& rotation() const { return rotation_; }
		void set_rotation(const glm::quat& rotation) { rotation_ = rotation; recalculate_view(); }

		const glm::mat4& projection_matrix() const { return projection_matrix_; }
		const glm::mat4& view_matrix() const { return view_matrix_; }
		const glm::mat4& view_projection_matrix() const { return view_projection_matrix_; }

		virtual bool on_window_resize(window_resize_event& event) = 0;

	private:
		void recalculate_view();

	protected:
		glm::mat4 projection_matrix_;
		glm::mat4 view_matrix_;
		glm::mat4 view_projection_matrix_;

		glm::vec3 position_ = { 0.0f, 0.0f, 0.0f };
		glm::quat rotation_ = { 1.0f, 0.0f, 0.0f, 0.0f };
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
