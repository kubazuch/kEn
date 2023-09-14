#include "kenpch.h"
#include "camera.h"

namespace kEn
{
	void camera::recalculate_view()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position()) * glm::mat4_cast(rotation_);

		view_matrix_ = glm::inverse(transform);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	orthographic_camera::orthographic_camera(float left, float right, float bottom, float top)
		: camera()
	{
		projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		view_matrix_ = glm::mat4(1.0f);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	void orthographic_camera::set_projection(float left, float right, float bottom, float top)
	{
		projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	perspective_camera::perspective_camera(float fov, float aspect, float zNear, float zFar)
		: camera()
	{
		projection_matrix_ = glm::perspective(fov, aspect, zNear, zFar);
		view_matrix_ = glm::mat4(1.0f);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	void perspective_camera::set_projection(float fov, float aspect, float zNear, float zFar)
	{
		projection_matrix_ = glm::perspective(fov, aspect, zNear, zFar);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}
}
