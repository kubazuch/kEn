#include "kenpch.h"
#include "opengl_buffer.h"

#include <glad/gl.h>

namespace kEn
{
	/*
	 *		VERTEX BUFFER
	 */

	opengl_vertex_buffer::opengl_vertex_buffer(void* vertices, uint32_t size)
		: renderer_id_(0)
	{
		glCreateBuffers(1, &renderer_id_);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	opengl_vertex_buffer::~opengl_vertex_buffer()
	{
		glDeleteBuffers(1, &renderer_id_);
	}

	void opengl_vertex_buffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
	}

	void opengl_vertex_buffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	/*
	 *		MUTABLE VERTEX BUFFER
	 */

	opengl_mutable_vertex_buffer::opengl_mutable_vertex_buffer(void* vertices, uint32_t size)
		: renderer_id_(0)
	{
		glCreateBuffers(1, &renderer_id_);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	opengl_mutable_vertex_buffer::~opengl_mutable_vertex_buffer()
	{
		glDeleteBuffers(1, &renderer_id_);
	}

	void opengl_mutable_vertex_buffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
	}

	void opengl_mutable_vertex_buffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void opengl_mutable_vertex_buffer::modify_data(std::function<void(void*)> fn) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		fn(ptr);
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	/*
	 *		INDEX BUFFER
	 */

	opengl_index_buffer::opengl_index_buffer(uint32_t* indices, uint32_t count)
		: renderer_id_(0), count_(count)
	{
		glGenBuffers(1, &renderer_id_);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	opengl_index_buffer::~opengl_index_buffer()
	{
		glDeleteBuffers(1, &renderer_id_);
	}

	void opengl_index_buffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
	}

	void opengl_index_buffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
