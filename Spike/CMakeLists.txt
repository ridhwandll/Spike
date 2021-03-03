include(${CMAKE_SOURCE_DIR}/scripts/CMakeUtils.cmake)

set(INCLUDE_DIRS
    src
    ../Spike-Editor/src
    vendor
    vendor/GLFW/include
    vendor/glad/include
    vendor/stb_image
    vendor/box2D/include
    vendor/mono/include
    vendor/imgui
    vendor/assimp/include
    vendor/ImGuizmo
    vendor/glm
    vendor/entt/include
    vendor/yaml-cpp/include
    vendor/tinyfiledialogs
)

set(LIB_LINKS
    GLFW
    glad
    box2D
    imgui
    yaml-cpp
    TinyFD
    ${CMAKE_SOURCE_DIR}/Spike/vendor/assimp/lib/assimp-vc142-mt.lib
    ${CMAKE_SOURCE_DIR}/Spike/vendor/mono/lib/Debug/mono-2.0-sgen.lib
    opengl32
)

if(WIN32)
    set(DX11_LIBS d3d11 dxgi dxguid d3dCompiler)
else()
    set(DX11_LIBS)
endif()

file(GLOB_RECURSE SOURCE_FILES
    src/*.cpp
    src/*.h
    vendor/stb_image/stb_image.h
    vendor/stb_image/stb_image.cpp
    vendor/ImGuizmo/ImGuizmo.h
    vendor/ImGuizmo/ImGuizmo.cpp
)
SOURCE_GROUP_BY_FOLDER(Spike)

add_library(Spike STATIC ${SOURCE_FILES})
target_include_directories(Spike PUBLIC ${INCLUDE_DIRS})
target_link_libraries(Spike PUBLIC ${LIB_LINKS} ${DX11_LIBS})
target_precompile_headers(Spike PUBLIC "src/spkpch.h")
set_property(TARGET Spike PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

target_compile_definitions(
    Spike

    PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "GLFW_INCLUDE_NONE"

    PUBLIC
    $<$<CONFIG:Debug>:SPK_DEBUG>
    $<$<CONFIG:Release>:SPK_RELEASE>

    $<$<CONFIG:RelWithDebInfo>:SPK_DEBUG>
    $<$<CONFIG:MinSizeRel>:SPK_RELEASE>
)
