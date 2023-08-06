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
			float vertices[3 * 3] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			};

			unsigned int indices[3] = { 0, 1, 2 };

			// Generate VAO
			glGenVertexArrays(1, &vertex_array_);
			glBindVertexArray(vertex_array_);

			vertex_buffer_ = vertex_buffer::create(vertices, sizeof vertices);

			// Set vertex attrib pointers
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

			index_buffer_ = index_buffer::create(indices, 3);

			std::string vertex_src = R"(
				#version 330 core
				
				layout(location = 0) in vec3 a_Position;
				
				out vec3 v_Position;

				void main() {
					v_Position = a_Position;
					gl_Position = vec4(a_Position, 1.0);
				}
			)";

			std::string fragment_src = R"(
				#version 330 core
				
				layout(location = 0) out vec4 color;
				
				in vec3 v_Position;

				void main() {
					color = vec4(v_Position * 0.5 + 0.5, 1.0);
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
