project "Spike"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	debugdir "%{wks.location}"
	disablewarnings { "4267", "4244", "4302", "4311", "4312" }
	linkoptions { "-IGNORE:4006" }

	pchheader "spkpch.h"
	pchsource "src/spkpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"src/**.c",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/assimp/include/**.hpp",
		"vendor/assimp/include/**.inl",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp",
		"vendor/tinyfiledialogs/tinyfiledialogs.c",
		"vendor/tinyfiledialogs/tinyfiledialogs.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs
	{
		"src",
		"../Spike-Editor/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.box2D}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.tinyfiledialogs}",
		"%{IncludeDir.FontAwesome}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"box2D",
		"%{LibraryDir.Assimp}",
		"%{LibraryDir.mono}",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		links
		{
			"d3d11.lib",
			"dxgi.lib",
			"dxguid.lib",
			"d3dCompiler.lib",
		}

	filter "configurations:Debug"
		defines "SPK_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SPK_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SPK_DIST"
		runtime "Release"
		optimize "on" 

	filter "files:**.c"
		flags {"NoPCH"}