project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Spike/vendor/spdlog/include",
		"%{wks.location}/Spike/src",
		"%{wks.location}/Spike/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
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