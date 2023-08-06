#include "kenpch.h"
#include "application.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "assert.h"
#include "kEn/event/application_events.h"

#ifdef KEN_PLATFORM_WIN
#	include "platform/win/win_window.h"
#endif

namespace kEn
{
	application* application::instance_ = nullptr;

	static GLenum tmp(shader_data_type type)
	{
		switch(type)
		{
		case shader_data_types::float_:	return GL_FLOAT;
		case shader_data_types::float2:	return GL_FLOAT;
		case shader_data_types::float3:	return GL_FLOAT;
		case shader_data_types::float4:	return GL_FLOAT;
		case shader_data_types::mat3:	return GL_FLOAT;
		case shader_data_types::mat4:	return GL_FLOAT;
		case shader_data_types::int_:	return GL_INT;
		case shader_data_types::int2:	return GL_INT;
		case shader_data_types::int3:	return GL_INT;
		case shader_data_types::int4:	return GL_INT;
		case shader_data_types::bool_:	return GL_BOOL;
		}

		return 0;
	}

	application::application()
	{
		KEN_CORE_ASSERT(!instance_, "App already exists!");
		instance_ = this;

		window_ = std::unique_ptr<window>(window::create());
		window_->set_event_handler([this](auto& event) { window_event_handler(event); });

		dispatcher_ = std::make_unique<event_dispatcher>();
		dispatcher_->subscribe<window_close_event>(KEN_EVENT_SUBSCRIBER(on_window_close));
		dispatcher_->subscribe<window_resize_event>(KEN_EVENT_SUBSCRIBER(on_window_resize));

		imgui_layer_ = new imgui_layer();
		push_overlay(imgui_layer_);

		//TODO: remove
		{
			float vertices[3 * (3 + 4)] = {
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			};

			unsigned int indices[3] = { 0, 1, 2 };

			// Generate VAO
			glGenVertexArrays(1, &vertex_array_);
			glBindVertexArray(vertex_array_);

			vertex_buffer_ = vertex_buffer::create(vertices, sizeof vertices);

			buffer_layout layout = {
				{shader_data_types::float3, "a_Position"},
				{shader_data_types::float4, "a_Color"}
			};
			vertex_buffer_->set_layout(layout);

			// Set vertex attrib pointers
			//glEnableVertexAttribArray(0);
			//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

			uint32_t index = 0;
			for(const auto& element : vertex_buffer_->layout())
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, 
					shader_data_types::get_component_count(element.type),
					tmp(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.stride(),
					(const void*)element.offset);
				index++;
			}

			index_buffer_ = index_buffer::create(indices, 3);

			std::string vertex_src = R"(
				#version 330 core
				
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;
				
				out vec4 v_Color;

				void main() {
					v_Color = a_Color;
					gl_Position = vec4(a_Position, 1.0);
				}
			)";

			std::string fragment_src = R"(
				#version 330 core
				
				layout(location = 0) out vec4 color;
				
				in vec4 v_Color;

				void main() {
					color = v_Color;
				}
			)";

			shader_ = shader::create("basic", vertex_src, fragment_src);
		}
	}

	void application::push_layer(layer* layer)
	{
		layer_stack_.push_layer(layer);
	}

	void application::push_overlay(layer* overlay)
	{
		layer_stack_.push_overlay(overlay);
	}

	void application::run()
	{
		while (running_)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			//TODO: remove
			{
				glBindVertexArray(vertex_array_);
				shader_->bind();
				vertex_buffer_->bind();
				index_buffer_->bind();
				glDrawElements(GL_TRIANGLES, index_buffer_->get_count(), GL_UNSIGNED_INT, nullptr);
				shader_->unbind();
			}

			for (layer* layer : layer_stack_)
				layer->on_update();

			imgui_layer_->begin();
			for (layer* layer : layer_stack_)
				layer->on_imgui();
			imgui_layer_->end();

			window_->on_update();
		}
	}

	void application::window_event_handler(base_event& e)
	{
		dispatcher_->dispatch(e);

		for (auto it = layer_stack_.rbegin(); it != layer_stack_.rend(); ++it)
		{
			if (e.handled)
				break;
			(*it)->on_event(e);
		}
	}

	bool application::on_window_close(window_close_event& e)
	{
		running_ = false;
		return true;
	}

	bool application::on_window_resize(window_resize_event& e)
	{
		// TODO: Renderer abstraction!
		glViewport(0, 0, e.width(), e.height());
		return true;
	}
}
