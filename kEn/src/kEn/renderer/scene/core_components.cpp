#include "kenpch.h"
#include "core_components.h"

#include "game_object.h"
#include "kEn/core/application.h"
#include "kEn/core/input.h"

namespace kEn
{
	void model_component::render(shader& shader)
	{
		if (!parent_.has_value()) return;
		model_->render(shader, transform());
	}

	std::shared_ptr<game_component> model_component::clone() const
	{
		return std::make_shared<model_component>(model_);
	}



	free_look_component::free_look_component(float sensitivity) : sensitivity_(sensitivity)
	{
		dispatcher_.subscribe<window_resize_event>(KEN_EVENT_SUBSCRIBER(on_window_resize));
		const window& main = kEn::application::instance().main_window();
		window_center_ = { main.width() / 2, main.height() / 2 };
	}

	void free_look_component::update(float delta)
	{
		glm::vec2 delta_pos = kEn::input::get_mouse_pos() - window_center_;
		bool rotY = delta_pos.x != 0;
		bool rotX = delta_pos.y != 0;


		if (rotY)
		{
			glm::quat rot({ 0, -glm::radians(delta_pos.x) * sensitivity_, 0 });
			transform().rotate(rot);
		}

		if (rotX)
		{
			glm::quat rot({ -glm::radians(delta_pos.y) * sensitivity_, 0, 0 });
			transform().rotate_local(rot);
		}

		if (rotX || rotY)
		{
			kEn::input::set_mouse_pos(window_center_);
		}
	}

	std::shared_ptr<game_component> free_look_component::clone() const
	{
		return std::make_shared<free_look_component>(sensitivity_);
	}

	bool free_look_component::on_window_resize(const kEn::window_resize_event& event)
	{
		window_center_ = glm::vec2(event.width() / 2, event.height() / 2);
		return false;
	}



	void free_move_component::update(float delta)
	{
		float move_amount = kEn::input::is_key_pressed(kEn::key::left_control) ? 3.f * delta * speed_ : delta * speed_;
		glm::vec3 direction{0.f};
		if (kEn::input::is_key_pressed(kEn::key::up) || kEn::input::is_key_pressed(kEn::key::w))
		{
			direction += transform().local_front();
		}
		if (kEn::input::is_key_pressed(kEn::key::down) || kEn::input::is_key_pressed(kEn::key::s))
		{
			direction -= transform().local_front();
		}
		if (kEn::input::is_key_pressed(kEn::key::right) || kEn::input::is_key_pressed(kEn::key::d))
		{
			direction += transform().local_right();
		}
		if (kEn::input::is_key_pressed(kEn::key::left) || kEn::input::is_key_pressed(kEn::key::a))
		{
			direction -= transform().local_right();
		}
		if (kEn::input::is_key_pressed(kEn::key::space) || kEn::input::is_key_pressed(kEn::key::q))
		{
			direction += world_y_ ? glm::vec3(0, 1, 0) : transform().local_up();
		}
		if (kEn::input::is_key_pressed(kEn::key::left_shift) || kEn::input::is_key_pressed(kEn::key::e))
		{
			direction -= world_y_ ? glm::vec3(0, 1, 0) : transform().local_up();
		}

		if (direction.x || direction.y || direction.z)
		{
			transform().fma(glm::normalize(direction), move_amount);
		}
	}

	std::shared_ptr<game_component> free_move_component::clone() const
	{
		return std::make_shared<free_move_component>(speed_, world_y_);
	}



	void look_at_component::set_target(const game_object& target)
	{
		target_ = target;
	}

	void look_at_component::update(float delta)
	{
		transform().look_at(target_.get().transform().pos());
	}

	std::shared_ptr<game_component> look_at_component::clone() const
	{
		return std::make_shared<look_at_component>(target_);
	}
}
