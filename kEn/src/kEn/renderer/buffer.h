#pragma once

namespace kEn
{
	class vertex_buffer
	{
	public:
		virtual ~vertex_buffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static std::unique_ptr<vertex_buffer> create(float* vertices, uint32_t size);
	};

	class index_buffer
	{
	public:
		virtual ~index_buffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t get_count() const = 0;

		static std::unique_ptr<index_buffer> create(uint32_t* indices, uint32_t size);
	};
}