cmake_minimum_required(VERSION 3.16)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
        cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(CMAKE_CXX_STANDARD 17)
project(Spike LANGUAGES C CXX)

# vendor
add_subdirectory(Spike/vendor/GLFW)
add_subdirectory(Spike/vendor/glad)
add_subdirectory(Spike/vendor/imgui)
add_subdirectory(Spike/vendor/yaml-cpp)
add_subdirectory(Spike/vendor/box2D)

# Spike
add_subdirectory(Spike)
add_subdirectory(Spike-Editor)
add_subdirectory(Sandbox)

set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT "SpikeEditor" )
