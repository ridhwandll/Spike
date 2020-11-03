workspace "LightEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "LightEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "LightEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "LightEngine/vendor/imgui"
IncludeDir["glm"] = "LightEngine/vendor/glm"

include "LightEngine/vendor/GLFW"
include "LightEngine/vendor/Glad"
include "LightEngine/vendor/imgui"

project "LightEngine"
	location "LightEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "lepch.h"
	pchsource "LightEngine/src/lepch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"LE_PLATFORM_WINDOWS",
			"LE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "LE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "LE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "LE_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"LightEngine/vendor/spdlog/include",
		"LightEngine/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"LightEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"LE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "LE_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "LE_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "LE_DIST"
		runtime "Release"
		optimize "On" 
		