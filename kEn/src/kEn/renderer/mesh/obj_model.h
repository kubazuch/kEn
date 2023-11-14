#pragma once
#include "glm/glm.hpp"
#include "kEn/renderer/buffer.h"
#include "kEn/renderer/vertex_array.h"

namespace kEn
{
	class obj_model
	{
	public:
		obj_model(const std::filesystem::path& path);


		std::shared_ptr<vertex_array> vertex_array_;

	public:
		static buffer_layout obj_layout;
		static const std::filesystem::path model_path;
	};

	struct obj_vertex
	{
		glm::vec3 pos;
		glm::vec2 tex_coord;
		glm::vec3 normal;
	};
}
