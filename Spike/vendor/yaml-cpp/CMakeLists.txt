project(yaml-cpp)

file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp" "include/*.h")
set(INCLUDE_DIRS include)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
include_directories(${INCLUDE_DIRS})