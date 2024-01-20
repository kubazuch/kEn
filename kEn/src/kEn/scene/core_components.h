#pragma once
#include "component.h"
#include "kEn/scene/camera/camera.h"
#include "kEn/event/application_events.h"
#include "kEn/scene/mesh/model.h"

namespace kEn
{
	class model_component : public game_component
	{
	public:
		explicit model_component(const std::shared_ptr<model>& model) : model_(model) {}

		void update(float delta) override {}
		void render(shader& shader) override;
		[[nodiscard]] std::shared_ptr<game_component> clone() const override;
	private:
		std::shared_ptr<model> model_;
	};

	class free_look_component : public game_component
	{
	public:
		free_look_component(float sensitivity);

		void update(float delta) override;
		void render(shader& shader) override {}
		[[nodiscard]] std::shared_ptr<game_component> clone() const override;

		float sensitivity() const {	return sensitivity_; }
		void set_sensitivity(float sensitivity)	{ sensitivity_ = sensitivity; }

		bool on_window_resize(const kEn::window_resize_event& event);

	private:
		float sensitivity_;
		float pitch_ = 0, yaw_ = 0;
		bool update_ = false;
		glm::vec2 window_center_;
	};

	class free_move_component : public game_component
	{
	public:
		explicit free_move_component(float speed, bool world_y = true) : speed_(speed), world_y_(world_y) {}

		void update(float delta) override;
		void render(shader& shader) override {}
		[[nodiscard]] std::shared_ptr<game_component> clone() const override;

		float speed() const { return speed_; }
		void set_speed(float speed) { speed_ = speed; }

	private:
		float speed_;
		bool world_y_;
	};

	class look_at_component: public game_component
	{
	public:
		look_at_component(const game_object& target) : target_(target) {}

		void set_target(const game_object& target);
		const game_object& target() const { return target_; }

		void update(float delta) override;
		void render(shader& shader) override {}
		[[nodiscard]] std::shared_ptr<game_component> clone() const override;

	private:
		std::reference_wrapper<const game_object> target_;
	};
}
