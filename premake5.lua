include "./vendor/premake/premake_customization/solution_items.lua"

workspace "LightEngine"
    architecture "x86_64"
    startproject "Lightning-Editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }
    solution_items
    {
        ".editorconfig"
    }
    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/LightEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/LightEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/LightEngine/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/LightEngine/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/LightEngine/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/LightEngine/vendor/entt/include"

group "Dependencies"
    include "vendor/premake"
    include "LightEngine/vendor/GLFW"
    include "LightEngine/vendor/Glad"
    include "LightEngine/vendor/imgui"
group ""

include "LightEngine"
include "Sandbox"
include "Lightning-Editor"