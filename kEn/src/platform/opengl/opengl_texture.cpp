#include "kenpch.h"
#include "opengl_texture.h"

#include <stb_image.h>

#include "imgui/imgui.h"
#include "glm/ext/scalar_common.hpp"
#include "kEn/core/assert.h"
#include "kEn/renderer/texture.h"

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

		static GLenum ken_filter_to_gl(texture_spec::filter filter, bool mipmap = false)
		{
			switch (filter)
			{
			case texture_spec::filter::NEAREST:
				return mipmap ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST;
			case texture_spec::filter::LINEAR:
			default:
				return mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
			}
		}

		static GLenum ken_wrap_to_gl(texture_spec::wrap wrap)
		{
			switch (wrap)
			{
			case texture_spec::wrap::CLAMP:
				return GL_CLAMP_TO_EDGE;
			case texture_spec::wrap::MIRRORED_REPEAT:
				return GL_MIRRORED_REPEAT;
			case texture_spec::wrap::REPEAT:
			default:
				return GL_REPEAT;
			}
		}
	}

	opengl_texture2D::opengl_texture2D(const texture_spec& specification)
		: spec_(specification)
	{
		internal_format_ = utils::ken_image_format_to_gl_internal(specification.format.value());
		data_format_ = utils::ken_image_format_to_gl_data(specification.format.value());

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
		glTextureStorage2D(renderer_id_, 1, internal_format_, spec_.width.value(), spec_.height.value());

		glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, utils::ken_filter_to_gl(specification.min_filter));
		glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, utils::ken_filter_to_gl(specification.mag_filter));
		glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, utils::ken_wrap_to_gl(specification.x_wrap));
		glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, utils::ken_wrap_to_gl(specification.y_wrap));
	}

	opengl_texture2D::opengl_texture2D(const std::filesystem::path& path, const texture_spec& spec)
		: spec_(spec), path_(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

		if(data)
		{
			is_loaded_ = true;
			spec_.width = width;
			spec_.height = height;

			GLenum internal_f = 0, data_f = 0;
			if(channels == 4)
			{
				internal_f = GL_RGBA8;
				data_f = GL_RGBA;
				spec_.format = image_format::RGBA8;
			}
			else if (channels == 3)
			{
				internal_f = GL_RGB8;
				data_f = GL_RGB;
				spec_.format = image_format::RGB8;
			}
			else if (channels == 1)
			{
				internal_f = GL_R8;
				data_f = GL_RED;
				spec_.format = image_format::R8;
			}

			KEN_CORE_INFO("Loaded texture with {} channels", channels);

			internal_format_ = internal_f;
			data_format_ = data_f;

			glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
			glTextureStorage2D(renderer_id_, spec_.mipmap_levels, internal_format_, spec_.width.value(), spec_.height.value());

			glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, utils::ken_filter_to_gl(spec.min_filter, spec.mipmap_levels > 1));
			glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, utils::ken_filter_to_gl(spec.mag_filter));
			glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, utils::ken_wrap_to_gl(spec.x_wrap));
			glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, utils::ken_wrap_to_gl(spec.y_wrap));

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

			glGenerateTextureMipmap(renderer_id_);

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
		KEN_CORE_ASSERT(size == spec_.width.value() * spec_.height.value() * bpp, "Data must be entire texture!");
		glTextureSubImage2D(renderer_id_, 0, 0, 0, spec_.width.value(), spec_.height.value(), data_format_, GL_UNSIGNED_BYTE, data);
	}

	void opengl_texture2D::bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, renderer_id_);
	}

	void opengl_texture2D::imgui()
	{
		float height = glm::max(glm::min((float) spec_.height.value(), 250.f), 100.f);
		float width = ( (float)spec_.width.value() ) / (float)spec_.height.value() * height;

		ImGui::Image((ImTextureID)renderer_id_, ImVec2{ width, height}, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });
	}

}
