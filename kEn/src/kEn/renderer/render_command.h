#pragma once
#include "renderer_api.h"

namespace kEn
{
	class render_command
	{
	public:
		static void init() { renderer_api_->init(); }
		static void set_viewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) { renderer_api_->set_viewport(x, y, w, h); }
		static void set_clear_color(const glm::vec4& color) { renderer_api_->set_clear_color(color); }
		static void clear() { renderer_api_->clear(); }

		static void draw_indexed(const std::shared_ptr<vertex_array>& vertex_array, uint32_t index_count = 0) { renderer_api_->draw_indexed(vertex_array, index_count); }
	private:
		static std::unique_ptr<renderer_api> renderer_api_;
	};
}
