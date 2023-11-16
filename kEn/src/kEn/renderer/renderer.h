#pragma once
#include "shader.h"
#include "vertex_array.h"
#include "kEn/camera/camera.h"
#include "kEn/core/transform.h"

namespace kEn
{
	class renderer
	{
	public:

		static void begin_scene(std::shared_ptr<camera> camera);
		static void end_scene();

		static void submit(shader& shader, const vertex_array& vertex_array, const transform& transform);
	private:
		struct scene_data
		{
			glm::mat4 VP_matrix;
		};

		static std::unique_ptr<scene_data> scene_data_;
	};
}
