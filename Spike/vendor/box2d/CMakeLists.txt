cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE box2D_source_files "src/*.cpp")
file(GLOB_RECURSE box2D_header_files "src/*.h")
file(GLOB_RECURSE box2D_include_files "include/*.h")

add_library(box2D STATIC
    ${box2D_source_files}
    ${box2D_header_files}
    ${box2D_include_files}
)

target_include_directories(box2D PUBLIC

    "include/"
    "src/"
)