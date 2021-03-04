# Copyright 2021 - Spike Technologies - All Rights Reserved
cmake_minimum_required(VERSION 3.16.0)

project(Spike)
set(CMAKE_CXX_STANDARD 17)

#Prevents in source build
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -g3 -O0 -Wall -pedantic -Wextra -m64 -mavx2 -mfma -ffast-math") #Using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -Wall -pedantic -Wextra -m64 -mavx2 -mfma -ffast-math") #Using GCC
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} /std:c++17 /W4 /arch:AVX2 /fp:fast /MP /DNOMINMAX") #Using Visual Studio C++
endif()

# vendor
add_subdirectory(Spike/vendor/GLFW)
add_subdirectory(Spike/vendor/glad)
add_subdirectory(Spike/vendor/imgui)
add_subdirectory(Spike/vendor/yaml-cpp)
add_subdirectory(Spike/vendor/box2D)
add_subdirectory(Spike/vendor/tinyfiledialogs)

# Spike
add_subdirectory(Spike)
add_subdirectory(Spike-Editor)
add_subdirectory(ScriptEngine)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "SpikeEditor")
