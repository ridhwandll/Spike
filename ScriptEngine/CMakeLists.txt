# Copyright 2021 - Spike Technologies - All Rights Reserved
cmake_minimum_required(VERSION 3.8)
file(GLOB_RECURSE SOURCE_FILES src/*.cs)
project("ScriptEngine" LANGUAGES CSharp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
