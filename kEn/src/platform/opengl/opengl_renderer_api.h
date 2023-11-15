#pragma once
#include "kEn/renderer/renderer_api.h"

namespace kEn
{
	class opengl_renderer_api : public kEn::renderer_api
	{
	public:
		~opengl_renderer_api() override = default;

		void init() override;
		void set_viewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) override;

		void set_clear_color(const glm::vec4& color) override;
		void clear() override;

		void draw_indexed(const vertex_array& vertex_array, uint32_t index_count) override;
		void set_wireframe(bool wireframe) override;
	};
}
