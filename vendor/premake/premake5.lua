project "Premake"
	kind "Utility"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	files {
		"%{wks.location}/**.lua"
	}

	postbuildmessage "Producing new project files..."
	postbuildcommands {
		"\"%{prj.location}premake5\" %{_ACTION} --file=\"%{wks.location}premake5.lua\""
	}