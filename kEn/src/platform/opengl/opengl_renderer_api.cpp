#include "kenpch.h"
#include "opengl_renderer_api.h"

#include <glad/glad.h>

namespace kEn
{
	renderer_api::api renderer_api::api_ = api::opengl;

	void gl_message_callback(unsigned src, unsigned type, unsigned id, unsigned lvl, int len, const char* msg, const void* params)
	{
		switch (lvl)
		{
		case GL_DEBUG_SEVERITY_HIGH:         KEN_CORE_CRITICAL(msg); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       KEN_CORE_ERROR(msg); return;
		case GL_DEBUG_SEVERITY_LOW:	         KEN_CORE_WARN(msg); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: KEN_CORE_TRACE(msg); return;
		}

		KEN_CORE_ASSERT(false, "Unknown message severity!");
	}

	void opengl_renderer_api::init()
	{
#	ifdef _KEN_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(gl_message_callback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#	endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_MULTISAMPLE);
	}

	void opengl_renderer_api::set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void opengl_renderer_api::set_clear_color(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void opengl_renderer_api::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void opengl_renderer_api::depth_testing(bool enabled)
	{
		if (enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		// glClear(GL_DEPTH_BUFFER_BIT);
	}

	void opengl_renderer_api::draw_indexed(const vertex_array& vertex_array, uint32_t index_count)
	{
		vertex_array.bind();
		uint32_t count = index_count ? index_count : vertex_array.index_buffer()->get_count();

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void opengl_renderer_api::draw_patches(const vertex_array& vertex_array, uint32_t vertex_count)
	{
		vertex_array.bind();
		glDrawArrays(GL_PATCHES, 0, vertex_count);
	}

	void opengl_renderer_api::set_tessellation_patch_vertices(uint32_t count)
	{
		glPatchParameteri(GL_PATCH_VERTICES, count);
	}

	void opengl_renderer_api::set_wireframe(bool wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	}
}
