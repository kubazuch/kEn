#include "kenpch.h"
#include "imgui_layer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "ImGuizmo.h"
#include "GLFW/glfw3.h"
#include "kEn/core/application.h"
#include "kEn/event/key_events.h"
#include "kEn/event/mouse_events.h"

namespace kEn
{
	imgui_layer::imgui_layer()
		: layer("ImGuiLayer")
	{
	}


	// https://github.com/ocornut/imgui/wiki/Getting-Started#example-if-you-are-using-glfw--openglwebgl
	void imgui_layer::on_attach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		application& app = application::instance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.main_window().native_window());
		ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void imgui_layer::on_detach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void imgui_layer::on_imgui()
	{
		//static bool show = true;
		//if(show)
		//	ImGui::ShowDemoWindow(&show); // Show demo window! :)
	}

	void imgui_layer::on_event(base_event& event)
	{
		ImGuiIO& io = ImGui::GetIO();
		event_dispatcher dispatcher;

		auto on_mouse_event = [&](base_event&)
			{
				return io.WantCaptureMouse;
			};

		auto on_keyboard_event = [&](base_event&)
			{
				return io.WantCaptureKeyboard;
			};

		dispatcher.subscribe<mouse_button_pressed_event>(on_mouse_event);
		dispatcher.subscribe<mouse_button_released_event>(on_mouse_event);
		dispatcher.subscribe<mouse_move_event>(on_mouse_event);
		dispatcher.subscribe<mouse_scroll_event>(on_mouse_event);
		dispatcher.subscribe<key_pressed_event>(on_keyboard_event);
		dispatcher.subscribe<key_released_event>(on_keyboard_event);
		dispatcher.subscribe<key_typed_event>(on_keyboard_event);

		dispatcher.dispatch(event);
	}

	void imgui_layer::begin()
	{
		// (Your code calls glfwPollEvents())
		// ...
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void imgui_layer::end()
	{
		// Rendering
		// (Your code clears your framebuffer, renders your other stuff etc.)
		ImGuiIO& io = ImGui::GetIO();
		application& app = application::instance();
		io.DisplaySize = ImVec2(static_cast<float>(app.main_window().width()), static_cast<float>(app.main_window().height()));

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// (Your code calls glfwSwapBuffers() etc.)

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(context);
		}
	}
}
