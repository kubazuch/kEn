#pragma once
#include "vertex.h"
#include "kEn/renderer/shader.h"
#include "kEn/renderer/texture.h"
#include "kEn/renderer/vertex_array.h"

namespace kEn
{
	class mesh
	{
	public:
		std::vector<vertex> vertices;
		std::vector<uint32_t> indices;
		kEn::material material;

		mesh(const std::vector<vertex>& vertices, const std::vector<uint32_t>& indices, kEn::material material);
		void render(shader& shader, const transform& transform) const;

	private:
		std::unique_ptr<vertex_array> vao_;
		void setup_mesh();

	public:
		static buffer_layout vertex_layout;
		static const std::filesystem::path model_path;
	};
}
