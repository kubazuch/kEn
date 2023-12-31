#pragma once

#include "kEn/core/assert.h"
#include "kEn/renderer/framebuffer.h"

namespace kEn
{
	class opengl_framebuffer : public framebuffer
	{
	public:
		opengl_framebuffer(const framebuffer_spec& spec);
		virtual ~opengl_framebuffer();

		void invalidate();

		void bind() override;
		void unbind() override;

		void resize(uint32_t width, uint32_t height) override;
		int read_pixel(uint32_t attachment_id, int x, int y) override;

		void clear_attachment(uint32_t attachment_id, int value) override;

		uint32_t get_color_attachment_renderer_id(uint32_t id) const override { KEN_CORE_ASSERT(id < color_attachments_.size()); return color_attachments_[id]; }

		const framebuffer_spec& get_spec() const override { return spec_; }

	private:
		uint32_t renderer_id_ = 0;
		framebuffer_spec spec_;

		std::vector<framebuffer_texture_spec> color_attachment_specs_;
		framebuffer_texture_spec depth_attachment_spec_ = framebuffer_texture_format::none;

		std::vector<uint32_t> color_attachments_;
		uint32_t depth_attachment_ = 0;
	};
}
