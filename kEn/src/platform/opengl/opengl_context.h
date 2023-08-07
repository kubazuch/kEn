#pragma once
#include "kEn/renderer/graphics_context.h"
#include "platform/win/win_window.h"

namespace kEn
{
	class opengl_context final : public graphics_context
	{
	public:
		opengl_context(GLFWwindow* window);

		void init() override;
		void swap_buffers() override;

	private:
		GLFWwindow* window_ptr_;
	};
}
