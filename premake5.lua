workspace "LightEngine"
    architecture "x86_64"
    startproject "Lightning-Editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
    
    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "LightEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "LightEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "LightEngine/vendor/imgui"
IncludeDir["glm"] = "LightEngine/vendor/glm"
IncludeDir["stb_image"] = "LightEngine/vendor/stb_image"
IncludeDir["entt"] = "LightEngine/vendor/entt/include"

group "Dependencies"
    include "LightEngine/vendor/GLFW"
    include "LightEngine/vendor/Glad"
    include "LightEngine/vendor/imgui"
group ""

project "LightEngine"
    location "LightEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "lepch.h"
    pchsource "LightEngine/src/lepch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}"
    }

    links 
    { 
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
        }

    filter "configurations:Debug"
        defines "LE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "LE_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "LightEngine/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}"
    }

    links
    {
        "LightEngine"
    }

    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "LE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "LE_DIST"
        runtime "Release"
        optimize "on"

project "Lightning-Editor"
    location "Lightning-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "LightEngine/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}"
    }

    links
    {
        "LightEngine"
    }

    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "LE_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "LE_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "LE_DIST"
        runtime "Release"
        optimize "on"