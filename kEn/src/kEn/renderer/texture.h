#pragma once

#include "kEn/core/core.h"
#include <filesystem>
#include <optional>

#define TEXTURE_TYPES(X)	\
	X(ambient_occlusion, aiTextureType_AMBIENT_OCCLUSION)	\
	X(diffuse, aiTextureType_DIFFUSE)				\
	X(height, aiTextureType_HEIGHT)				\
	X(normal, aiTextureType_NORMALS)				\
	X(specular, aiTextureType_SPECULAR)

namespace kEn
{
	class texture;

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
		enum class filter
		{
			LINEAR,
			NEAREST
		};

		enum class wrap
		{
			REPEAT,
			CLAMP,
			MIRRORED_REPEAT
		};

		std::optional<uint32_t> width;
		std::optional<uint32_t> height;
		std::optional<image_format> format;

		uint32_t mipmap_levels = 1;

		filter min_filter = filter::LINEAR;
		filter mag_filter = filter::LINEAR;
		wrap x_wrap = wrap::REPEAT;
		wrap y_wrap = wrap::REPEAT;

		texture_spec& set_mipmap_levels(uint32_t l) { mipmap_levels = l; return *this; }
		texture_spec& set_min_filter(filter f) { min_filter = f; return *this; }
		texture_spec& set_mag_filter(filter f) { mag_filter = f; return *this; }
		texture_spec& set_x_wrap(wrap f) { x_wrap = f; return *this; }
		texture_spec& set_y_wrap(wrap f) { y_wrap = f; return *this; }
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
	};

	class texture2D : public texture
	{
	public:
		static std::shared_ptr<texture2D> create(const texture_spec& spec);
		static std::shared_ptr<texture2D> create(const std::filesystem::path& path, const texture_spec& spec = texture_spec());

		static const std::filesystem::path texture_path;

	private:
		static std::unordered_map<std::filesystem::path, std::shared_ptr<texture2D>> loaded_resources_;
	};
}
