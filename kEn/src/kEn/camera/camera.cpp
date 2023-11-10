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
		: camera(), left_(left), right_(right), bottom_(bottom), top_(top)
	{
		projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		view_matrix_ = glm::mat4(1.0f);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	void orthographic_camera::set_projection(float left, float right, float bottom, float top)
	{
		left_ = left; right_ = right; bottom_ = bottom; top_ = top;
		projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	bool orthographic_camera::on_window_resize(window_resize_event& event)
	{
		float aspect = (float)event.width() / (float)event.height();
		left_ = -aspect; right_ = aspect;
		projection_matrix_ = glm::ortho(left_, right_, bottom_, top_, -1.0f, 1.0f);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
		return true;
	}



	perspective_camera::perspective_camera(float fov, float aspect, float zNear, float zFar)
		: camera(), fov_(fov), aspect_(aspect), zNear_(zNear), zFar_(zFar)
	{
		projection_matrix_ = glm::perspective(fov, aspect, zNear, zFar);
		view_matrix_ = glm::mat4(1.0f);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	void perspective_camera::set_projection(float fov, float aspect, float zNear, float zFar)
	{
		fov_ = fov; aspect_ = aspect; zNear_ = zNear; zFar_ = zFar;
		projection_matrix_ = glm::perspective(fov, aspect, zNear, zFar);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	bool perspective_camera::on_window_resize(window_resize_event& event)
	{
		aspect_ = (float)event.width() / (float)event.height();
		projection_matrix_ = glm::perspective(fov_, aspect_, zNear_, zFar_);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;

		return true;
	}
}
