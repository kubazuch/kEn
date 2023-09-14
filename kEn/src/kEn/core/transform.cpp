#include "kenpch.h"
#include "transform.h"

namespace kEn
{
	transform::transform()
		: parent_(nullptr), pos_(0.0f, 0.0f, 0.0f), rot_(1.0f, 0.0f, 0.0f, 0.0f), scale_(1.0f, 1.0f, 1.0f)
	{
	}

	transform::~transform()
	{
		for (const auto child : children_)
		{
			child->set_parent(nullptr);
		}

		if (parent_)
			set_parent(nullptr);
	}

	void transform::set_dirty()
	{
		if (dirty_) return;

		dirty_ = true;
		inverse_dirty_ = true;

		for(const auto child : children_)
		{
			child->set_dirty();
		}
	}

	void transform::set_parent(transform* parent)
	{
		if (parent_)
			parent_->children_.remove(this);

		parent_ = parent;

		if (parent_)
			parent_->children_.push_back(this);

		set_dirty();
	}

	glm::mat4 transform::local_to_parent_matrix() const
	{
		return glm::translate(glm::mat4(1.0f), pos_) * glm::mat4_cast(rot_) * glm::scale(glm::mat4(1.0f), scale_);
	}

	glm::mat4 transform::local_to_world_matrix() const
	{
		if (!dirty_) return model_mat_;
		
		model_mat_ = local_to_parent_matrix();
		if(parent_)
			model_mat_ = parent_->local_to_world_matrix() * model_mat_;

		dirty_ = false;
		return model_mat_;
	}

	glm::mat4 transform::world_to_local_matrix() const
	{
		if (!inverse_dirty_) return inv_model_mat_;

		inv_model_mat_ = glm::inverse(model_mat_);
		inverse_dirty_ = false;
		return inv_model_mat_;
	}

	void transform::rotate(const glm::vec3& axis, float angle)
	{
		rot_ = glm::rotate(rot_, angle, axis);
		dirty_ = true;
	}

	void transform::rotate(const glm::quat& rotation)
	{
		rot_ = glm::normalize(rotation * rot_);
		dirty_ = true;
	}

	void transform::look_at(const glm::vec3& point, const glm::vec3& up)
	{
		rot_ = glm::quatLookAt(point - pos_, up);
		dirty_ = true;
	}
}
