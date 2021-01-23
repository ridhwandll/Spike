project "Spike"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	debugdir "%{wks.location}"

	pchheader "spkpch.h"
	pchsource "src/spkpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/assimp/include/**.hpp",
		"vendor/assimp/include/**.inl",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp"
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
		"vendor/spdlog/include",
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
		"%{IncludeDir.FontAwesome}"
	}

	libdirs
	{
		"%{wks.location}/Spike/vendor/assimp/lib"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"assimp-vc142-mt.lib",
		"box2D",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
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