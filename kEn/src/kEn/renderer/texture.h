#pragma once

#include "kEn/core/core.h"
#include <filesystem>

#define TEXTURE_TYPES(X)	\
	X(ambient_occlusion, aiTextureType_AMBIENT_OCCLUSION)	\
	X(diffuse, aiTextureType_DIFFUSE)				\
	X(height, aiTextureType_HEIGHT)				\
	X(normal, aiTextureType_NORMALS)				\
	X(specular, aiTextureType_SPECULAR)

namespace kEn
{
	enum class image_format
	{
		none = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct texture_spec
	{
		uint32_t width = 1;
		uint32_t height = 1;
		image_format format = image_format::RGBA8;
	};

	using texture_type_t = uint8_t;

	namespace texture_type
	{
#		define ENUM_ENTRY(name, x)	name,
#		define CASE_ENTRY(name, x)	case name: return #name;

		enum : texture_type_t {
			TEXTURE_TYPES(ENUM_ENTRY)
		};

		inline const char* name_of(const texture_type_t type)
		{
			switch(type)
			{
			TEXTURE_TYPES(CASE_ENTRY)
			default:
				return "INVALID";
			}
		}
	}

	class texture
	{
	public:
		virtual ~texture() = default;

		virtual const texture_spec& get_specification() const = 0;

		virtual uint32_t width() const = 0;
		virtual uint32_t height() const = 0;
		virtual uint32_t renderer_id() const = 0;

		virtual const std::filesystem::path& path() const = 0;
		virtual void set_data(void* data, uint32_t size) = 0;
		virtual void bind(uint32_t slot = 0) const = 0;
		virtual bool is_loaded() const = 0;
		virtual bool operator==(const texture& other) const = 0;

		void set_type(texture_type_t type) { type_ = type; }
		texture_type_t type() const { return type_;	}

	private:
		texture_type_t type_ = texture_type::diffuse;
	};

	class texture2D : public texture
	{
	public:
		static std::shared_ptr<texture2D> create(const texture_spec& spec);
		static std::shared_ptr<texture2D> create(const std::filesystem::path& path);

		static const std::filesystem::path texture_path;
	};
}
