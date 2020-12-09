cmake_minimum_required(VERSION 3.12)
project(LightEngine)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

FILE(GLOB GLM_HPP_FILES "LightEngine/vendor/glm/glm/*.hpp")
FILE(GLOB GLM_INL_FILES "LightEngine/vendor/glm/glm/*.inl")

add_library(
    LightEngine

    "LightEngine/src/lepch.cpp"
    "LightEngine/src/lepch.h"
    "LightEngine/src/LightEngine.h"
    "LightEngine/vendor/stb_image/stb_image.cpp"
    "LightEngine/vendor/stb_image/stb_image.h"
    ${GLM_HPP_FILES}
    ${GLM_INL_FILES}
)

target_link_libraries(LightEngine GLFW Glad ImGui opengl32)

target_include_directories(LightEngine PUBLIC
    LightEngine/vendor/GLFW/include
    LightEngine/vendor/Glad/include
    LightEngine/vendor/imgui
    LightEngine/vendor/glm
    LightEngine/vendor/stb_image
    LightEngine/vendor/spdlog/include
    LightEngine/
)

target_compile_definitions(LightEngine PUBLIC _CRT_SECURE_NO_WARNINGS)

add_executable(
    Sandbox
    Sandbox/src/Sandbox2D.cpp
    Sandbox/src/Sandbox2D.h
    Sandbox/src/SandboxApp.cpp
)

if (WIN32)
    target_compile_definitions(LightEngine PUBLIC
        LE_PLATFORM_WINDOWS
        LE_BUILD_DLL
        GLFW_INCLUDE_NONE
    )
    target_compile_definitions(Sandbox PUBLIC LE_PLATFORM_WINDOWS)
endif()

if (CMAKE_BUILD_TYPE EQUAL "Debug")
    target_compile_definitions(LightEngine PUBLIC LE_DEBUG)
    target_compile_definitions(Sandbox PUBLIC LE_DEBUG)
endif()

if (CMAKE_BUILD_TYPE EQUAL "Release")
    target_compile_definitions(LightEngine PUBLIC LE_RELEASE)
    target_compile_definitions(Sandbox PUBLIC LE_RELEASE)
endif()

target_include_directories(Sandbox PUBLIC
    LightEngine/vendor/spdlog/include
    LightEngine/src
    LightEngine/vendor
)

target_link_libraries(Sandbox LightEngine) 