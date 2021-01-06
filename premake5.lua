include "./vendor/premake/premake_customization/clean_build.lua"

workspace "Spike"
    architecture "x86_64"
    startproject "Spike-Editor"

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
IncludeDir["GLFW"] = "%{wks.location}/Spike/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Spike/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Spike/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Spike/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Spike/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Spike/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Spike/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Spike/vendor/ImGuizmo"
IncludeDir["Assimp"] = "%{wks.location}/Spike/vendor/assimp/include"
IncludeDir["FontAwesome"] = "%{wks.location}/Spike-Editor/src"

group "Dependencies"
    include "Spike/vendor/GLFW"
    include "Spike/vendor/Glad"
    include "Spike/vendor/imgui"
    include "Spike/vendor/yaml-cpp"
group ""

include "Spike"
include "Sandbox"
include "Spike-Editor"