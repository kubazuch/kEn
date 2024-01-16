#pragma once
#include "shader.h"
#include "vertex_array.h"
#include "kEn/core/transform.h"
#include "scene/core_components.h"

namespace kEn
{
	class renderer
	{
	public:

		static void begin_scene(std::shared_ptr<camera> camera);
		static void end_scene();

		static void submit(shader& shader, const vertex_array& vertex_array, const transform& transform);
		static void submit_tessellated(shader& shader, const vertex_array& vertex_array, const uint32_t& count, const transform& transform);
	private:
		struct scene_data
		{
			glm::mat4 VP_matrix;
			glm::vec3 camera_pos;
		};

		static std::unique_ptr<scene_data> scene_data_;
	};
}
