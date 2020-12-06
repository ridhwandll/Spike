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
IncludeDir["GLFW"] = "%{wks.location}/LightEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/LightEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/LightEngine/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/LightEngine/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/LightEngine/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/LightEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/LightEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/LightEngine/vendor/ImGuizmo"

group "Dependencies"
    include "LightEngine/vendor/GLFW"
    include "LightEngine/vendor/Glad"
    include "LightEngine/vendor/imgui"
    include "LightEngine/vendor/yaml-cpp"
group ""

include "LightEngine"
include "Sandbox"
include "Lightning-Editor"