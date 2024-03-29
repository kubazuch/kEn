#pragma once

#include <glm/glm.hpp>

#include "vertex_array.h"

namespace kEn
{
	class renderer_api
	{
	public:
		enum class api
		{
			none = 0,
			opengl = 1
		};

	public:
		virtual ~renderer_api() = default;

		virtual void init() = 0;
		virtual void set_viewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
		virtual void set_clear_color(const glm::vec4& color) = 0;
		virtual void clear() = 0;
		virtual void depth_testing(bool enabled) = 0;

		virtual void draw_indexed(const vertex_array& vertex_array, uint32_t index_count = 0) = 0;
		virtual void draw_patches(const vertex_array& vertex_array, uint32_t vertex_count = 0) = 0;

		virtual void set_tessellation_patch_vertices(uint32_t count = 4) = 0;

		virtual void set_wireframe(bool wireframe) = 0;

		static api get_api() { return api_; }
		static std::unique_ptr<renderer_api> create();
	private:
		static api api_;
	};
}
