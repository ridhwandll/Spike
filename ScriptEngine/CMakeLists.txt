cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE SOURCE_FILES src/*.cs)

project("ScriptEngine" LANGUAGES CSharp CXX)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
