include(${CMAKE_SOURCE_DIR}/scripts/CMakeUtils.cmake)

set(INCLUDE_DIRS src Spike/vendor)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
SOURCE_GROUP_BY_FOLDER(SpikeEditor)

add_executable(SpikeEditor ${SOURCE_FILES})
target_link_libraries(SpikeEditor PRIVATE Spike)
target_include_directories(SpikeEditor PRIVATE ${INCLUDE_DIRS})

file(COPY ${CMAKE_SOURCE_DIR}/Spike/vendor/assimp/lib/assimp-vc142-mt.dll DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
file(COPY ${CMAKE_SOURCE_DIR}/Spike/vendor/assimp/lib/assimp-vc142-mt.dll DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Dist")
file(COPY ${CMAKE_SOURCE_DIR}/Spike/vendor/assimp/lib/assimp-vc142-mt.dll DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release")

file(COPY ${CMAKE_SOURCE_DIR}/Spike/vendor/mono/bin/Debug/mono-2.0-sgen.dll DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
file(COPY ${CMAKE_SOURCE_DIR}/Spike/vendor/mono/bin/Debug/mono-2.0-sgen.dll DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Dist")
file(COPY ${CMAKE_SOURCE_DIR}/Spike/vendor/mono/bin/Debug/mono-2.0-sgen.dll DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release")

set_property(TARGET SpikeEditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SpikeEditor)
