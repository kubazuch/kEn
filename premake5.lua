include "dependencies.lua"

workspace "kEn"
	architecture "x86_64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "kEn/vendor/glfw.lua"
	include "kEn/vendor/glad.lua"
group ""

include "kEn"
include "Sandbox"

