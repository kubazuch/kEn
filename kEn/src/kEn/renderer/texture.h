#pragma once

#include "kEn/core/core.h"
#include <filesystem>

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
		static std::shared_ptr<texture2D> create(const std::filesystem::path& path);

		static const std::filesystem::path texture_path;
	};
}
