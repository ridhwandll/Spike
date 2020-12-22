  
file(GLOB_RECURSE spike_source_files "src/Spike/*.cpp")
file(GLOB_RECURSE spike_header_files "src/Spike/*.h")

set(SPK_CROSS_PLATFORM_FILES
	"src/Spike.h"
	"src/spkpch.cpp"
	"src/spkpch.h"
	
	${spike_source_files}
	${spike_header_files}

	"vendor/stb_image/stb_image.h"
	"vendor/stb_image/stb_image.cpp"
)

file(GLOB_RECURSE opengl_platform_source_files "src/Platform/OpenGL/*.cpp")
file(GLOB_RECURSE opengl_platform_header_files "src/Platform/OpenGL/*.h")

file(GLOB_RECURSE win32_platform_source_files "src/Platform/Windows/*.cpp")
file(GLOB_RECURSE win32_platform_header_files "src/Platform/Windows/*.h")

if(WIN32)
    set(SPK_OS_FILES ${win32_platform_source_files} ${win32_platform_header_files})
endif()

add_library(Spike STATIC ${SPK_CROSS_PLATFORM_FILES} ${SPK_OS_FILES}
    ${opengl_platform_source_files}
    ${opengl_platform_header_files}

    "vendor/ImGuizmo/ImGuizmo.h"
    "vendor/ImGuizmo/ImGuizmo.cpp"
)

target_precompile_headers(Spike PUBLIC "src/spkpch.h")

target_include_directories(Spike PRIVATE
    "res"
    "vendor/GLFW/include"
    "vendor/glad/include"
    "vendor/stb_image"
)

target_include_directories(Spike PUBLIC
    "src"
    "../Spike-Editor/src"
    "vendor"
    "vendor/spdlog/include"
    "vendor/imgui"
    "vendor/ImGuizmo"
    "vendor/glm"
    "vendor/entt/include"
    "vendor/yaml-cpp/include"
)

target_compile_definitions(Spike PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "GLFW_INCLUDE_NONE"
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions("SPK_DEBUG")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions("SPK_RELEASE")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Dist")
    add_compile_definitions("SPK_DIST")
endif()

set_property(TARGET Spike PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_link_libraries(Spike PRIVATE glfw glad)
target_link_libraries(Spike PUBLIC imgui yaml-cpp)