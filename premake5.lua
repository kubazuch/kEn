workspace "kEn"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glfw"] = "kEn/vendor/GLFW/include"

include "kEn/vendor/glfw.lua"

project "kEn"
	location "kEn"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	pchheader "kenpch.h"
	pchsource "kEn/src/kenpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.glfw}"
	}

	links {
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"KEN_PLATFORM_WIN",
			"KEN_DLL"
		}

		postbuildcommands {
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "KEN_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "KEN_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "KEN_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"kEn/vendor/spdlog/include",
		"kEn/src",
		"kEn/vendor"
	}

	links {
		"kEn"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines {
			"KEN_PLATFORM_WIN"
		}

	filter "configurations:Debug"
		defines "KEN_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "KEN_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "KEN_DIST"
		runtime "Release"
		optimize "On"
