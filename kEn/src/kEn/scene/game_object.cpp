#include "kenpch.h"
#include "game_object.h"

#include "kEn/renderer/renderer.h"
#include "kEn/scene/mesh/obj_model.h"

namespace kEn
{
	game_object::game_object(glm::vec3 pos, glm::quat rot, glm::vec3 scale)
		: transform_(pos, rot, scale)
	{

	}

	game_object::~game_object()
	{
		for (const auto child : children_)
		{
			child.get().parent_.reset();
		}

		if (parent_.has_value())
			std::erase_if(parent_.value().get().children_, [this](auto ref) { return std::addressof(ref.get()) == this; });
	}

	game_object& game_object::add_child(game_object& child)
	{
		if(child.parent_.has_value())
			std::erase_if(child.parent_.value().get().children_, [this](auto ref) { return std::addressof(ref.get()) == this; });

		child.parent_ = *this;
		child.transform().set_parent(transform_);

		children_.emplace_back(child);

		return *this;
	}

	game_object& game_object::add_children(std::initializer_list<std::reference_wrapper<game_object>> children)
	{
		for(auto child : children)
		{
			add_child(child);
		}

		return *this;
	}

	game_object& game_object::add_component(std::shared_ptr<game_component> to_add)
	{
		to_add->parent_ = *this;

		components_.push_back(std::move(to_add));

		return *this;
	}

	game_object& game_object::add_components(std::initializer_list<std::shared_ptr<game_component>> components)
	{
		for (auto& component : components)
		{
			add_component(component);
		}

		return *this;
	}

	void game_object::render(shader& shader) const
	{
		for (auto& component : components_)
		{
			component->render(shader);
		}
	}

	void game_object::render_all(shader& shader) const
	{
		render(shader);

		for (const auto child : children_) 
			child.get().render_all(shader);
	}

	void game_object::update(double delta)
	{
		for (auto& component : components_)
		{
			component->update(delta);
		}
	}

	void game_object::update_all(double delta)
	{
		update(delta);

		for (const auto child : children_)
			child.get().update_all(delta);
	}

	void game_object::on_event(base_event& event)
	{
		for (const auto& component : components_)
		{
			component->on_event(event);
			if (event.handled)
				return;
		}


		for (const auto& child : children_)
		{
			child.get().on_event(event);
			if (event.handled)
				return;
		}
	}
}
