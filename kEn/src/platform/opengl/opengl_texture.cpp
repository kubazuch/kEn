#include "kenpch.h"
#include "opengl_texture.h"

#include <stb_image.h>

#include "kEn/core/assert.h"

namespace kEn
{
	namespace utils
	{
		static GLenum ken_image_format_to_gl_data(image_format format)
		{
			switch(format)
			{
			case image_format::RGB8: return GL_RGB;
			case image_format::RGBA8: return GL_RGBA;
			}

			KEN_CORE_ASSERT(false);
			return 0;
		}

		static GLenum ken_image_format_to_gl_internal(image_format format)
		{
			switch (format)
			{
			case image_format::RGB8: return GL_RGB8;
			case image_format::RGBA8: return GL_RGBA8;
			}

			KEN_CORE_ASSERT(false);
			return 0;
		}
	}

	opengl_texture2D::opengl_texture2D(const texture_spec& specification)
		: spec_(specification), width_(specification.width), height_(specification.height)
	{
		internal_format_ = utils::ken_image_format_to_gl_internal(specification.format);
		data_format_ = utils::ken_image_format_to_gl_data(specification.format);

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
		glTextureStorage2D(renderer_id_, 1, internal_format_, width_, height_);

		glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	opengl_texture2D::opengl_texture2D(const std::filesystem::path& path)
		: path_(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

		if(data)
		{
			is_loaded_ = true;
			width_ = width;
			height_ = height;

			GLenum internal_f = 0, data_f = 0;
			if(channels == 4)
			{
				internal_f = GL_RGBA8;
				data_f = GL_RGBA;
			}
			else if (channels == 3)
			{
				internal_f = GL_RGB8;
				data_f = GL_RGB;
			}
			else if (channels == 1)
			{
				internal_f = GL_R8;
				data_f = GL_RED;
			}

			KEN_CORE_INFO("Loaded texture with {} channels", channels);

			internal_format_ = internal_f;
			data_format_ = data_f;

			glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
			glTextureStorage2D(renderer_id_, 1, internal_format_, width_, height_);

			glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

			if(channels == 4)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			}
			else if(channels == 2)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			}
			else
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			}
			glTextureSubImage2D(renderer_id_, 0, 0, 0, width, height, data_f, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
		else
		{
			KEN_CORE_CRITICAL("stb_image load error: {0}", stbi_failure_reason());
		}
	}

	opengl_texture2D::~opengl_texture2D()
	{
		glDeleteTextures(1, &renderer_id_);
	}

	void opengl_texture2D::set_data(void* data, uint32_t size)
	{
		uint32_t bpp = data_format_ == GL_RGBA ? 4 : 3;
		KEN_CORE_ASSERT(size == width_ * height_ * bpp, "Data must be entire texture!");
		glTextureSubImage2D(renderer_id_, 0, 0, 0, width_, height_, data_format_, GL_UNSIGNED_BYTE, data);
	}

	void opengl_texture2D::bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, renderer_id_);
	}
}
