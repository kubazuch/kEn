project "Glad"
	location "Glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
	targetdir ("%{prj.name}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{prj.name}/obj/" .. outputdir .. "/%{prj.name}")
	
	files {
		"%{prj.name}/include/glad/glad.h",
        "%{prj.name}/include/KHR/khrplatform.h",
        "%{prj.name}/src/glad.c"
	}

	includedirs {
		"%{prj.name}/include"
	}
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"