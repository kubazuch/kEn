project "kEn"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	pchheader "kenpch.h"
	pchsource "src/kenpch.cpp"

	files {
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",

		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs {
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.imguizmo}",
		"%{IncludeDir.assimp}"
	}

	links {
		"GLFW",
		"Glad",
		"imgui",
		"assimp",
		"opengl32.lib"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
	}

	filter "files:vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines {
			"KEN_PLATFORM_WIN"
		}

	filter "configurations:Debug"
		defines "_KEN_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "_KEN_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "_KEN_DIST"
		runtime "Release"
		optimize "on"