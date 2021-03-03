#Example Project
cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE ENGINE_SOURCE_FILES ../ScriptEngine/src/*.cs)
project("ScriptEngine" LANGUAGES CSharp CXX)
add_library("ScriptEngine" SHARED ${ENGINE_SOURCE_FILES})

set_target_properties("ScriptEngine"
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

file(GLOB_RECURSE APP_SOURCE_FILES src/*.cs)
project("ExampleApp" LANGUAGES CSharp CXX)
add_library("ExampleApp" SHARED ${APP_SOURCE_FILES})
target_link_libraries("ExampleApp" ScriptEngine)

set_target_properties("ExampleApp"
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)
