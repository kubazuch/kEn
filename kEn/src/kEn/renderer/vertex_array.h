#pragma once
#include "buffer.h"

namespace kEn
{
	class vertex_array
	{
	public:
		virtual ~vertex_array() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void add_vertex_buffer(const std::shared_ptr<vertex_buffer>& vertex_buf) = 0;
		virtual void set_index_buffer(const std::shared_ptr<index_buffer>& index_buf) = 0;

		virtual const std::vector<std::shared_ptr<vertex_buffer>>& vertex_buffers() const = 0;
		virtual const std::shared_ptr<index_buffer>& index_buffer() const = 0;

		static std::unique_ptr<vertex_array> create();
	};
}
