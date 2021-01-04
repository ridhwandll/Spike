project "Spike-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	debugdir "%{wks.location}"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Spike/vendor/spdlog/include",
		"%{wks.location}/Spike/src",
		"../Spike-Editor/src",
		"%{wks.location}/Spike/vendor",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.FontAwesome}"
	}

	links
	{
		"Spike"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "SPK_DEBUG"
		debugdir "%{wks.location}"
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