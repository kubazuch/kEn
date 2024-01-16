project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp",
	}

	includedirs {
		"%{wks.location}/kEn/vendor/spdlog/include",
		"%{wks.location}/kEn/src",
		"%{wks.location}/kEn/vendor",
		"%{IncludeDir.glm}"
	}

	links {
		"kEn"
	}

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
