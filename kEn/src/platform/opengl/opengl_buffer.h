#pragma once

#include "kEn/renderer/buffer.h"

namespace kEn
{
	class opengl_vertex_buffer : public vertex_buffer
	{
	public:
		opengl_vertex_buffer(float* vertices, uint32_t size);
		~opengl_vertex_buffer() override;

		void bind() const override;
		void unbind() const override;

		const buffer_layout& layout() const override { return layout_; }
		void set_layout(const buffer_layout& layout) override { layout_ = layout; }

	private:
		uint32_t renderer_id_;
		buffer_layout layout_;
	};

	class opengl_mutable_vertex_buffer final : public mutable_vertex_buffer
	{
	public:
		opengl_mutable_vertex_buffer(float* vertices, uint32_t size);
		~opengl_mutable_vertex_buffer() override;

		void bind() const override;
		void unbind() const override;
		void modify_data(std::function<void(void*)> fn) const override;

		const buffer_layout& layout() const override { return layout_; }
		void set_layout(const buffer_layout& layout) override { layout_ = layout; }

	private:
		uint32_t renderer_id_;
		buffer_layout layout_;
	};

	class opengl_index_buffer final : public index_buffer
	{
	public:
		opengl_index_buffer(uint32_t* indices, uint32_t count);
		~opengl_index_buffer() override;

		void bind() const override;
		void unbind() const override;

		uint32_t get_count() const override { return count_; }

	private:
		uint32_t renderer_id_;
		uint32_t count_;
	};
}