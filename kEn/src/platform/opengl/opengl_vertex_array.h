#pragma once
#include "kEn/renderer/vertex_array.h"

namespace kEn
{
	class opengl_vertex_array : public vertex_array
	{
	public:
		opengl_vertex_array();
		~opengl_vertex_array() override;

		void bind() const override;
		void unbind() const override;

		void add_vertex_buffer(const std::shared_ptr<vertex_buffer>& vertex_buf) override;
		void set_index_buffer(const std::shared_ptr<kEn::index_buffer>& index_buf) override;

		const std::vector<std::shared_ptr<vertex_buffer>>& vertex_buffers() const override { return vertex_buffers_; }
		const std::shared_ptr<kEn::index_buffer>& index_buffer() const override { return index_buffer_; }
	private:
		uint32_t renderer_id_;
		uint32_t vertex_buffer_index_ = 0;

		std::vector<std::shared_ptr<vertex_buffer>> vertex_buffers_;
		std::shared_ptr<kEn::index_buffer> index_buffer_;
	};
}
