#pragma once

#include "kEn/renderer/texture.h"
#include <filesystem>
#include <glad/gl.h>

namespace kEn
{
	class opengl_texture2D : public texture2D
	{
	public:
		opengl_texture2D(const texture_spec& specification);
		opengl_texture2D(const std::filesystem::path& path, const texture_spec& specification = texture_spec());
		virtual ~opengl_texture2D() override;

		const texture_spec& get_specification() const override { return spec_; }
		uint32_t width() const override { return spec_.width.value(); }
		uint32_t height() const override { return spec_.height.value(); }
		uint32_t renderer_id() const override { return renderer_id_; }
		const std::filesystem::path& path() const override { return path_; }
		void set_data(void* data, uint32_t size) override;
		void bind(uint32_t slot) const override;
		bool is_loaded() const override { return is_loaded_; }
		bool operator==(const texture& other) const override { return renderer_id_ == other.renderer_id(); }

		void imgui() override;
	private:
		texture_spec spec_;
		std::filesystem::path path_;
		bool is_loaded_ = false;
		uint32_t renderer_id_;
		GLenum internal_format_, data_format_;
	};
}
