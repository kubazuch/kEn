project "kEn"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	pchheader "kenpch.h"
	pchsource "src/kenpch.cpp"

	files {
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl"
	}

	includedirs {
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}"
	}

	links {
		"GLFW",
		"Glad",
		"imgui",
		"opengl32.lib"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"KEN_PLATFORM_WIN"
		}

	filter "configurations:Debug"
		defines "KEN_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "KEN_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "KEN_DIST"
		runtime "Release"
		optimize "on"