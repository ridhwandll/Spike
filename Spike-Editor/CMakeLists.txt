file(GLOB_RECURSE spikeeditor_source_files "src/*.cpp")
file(GLOB_RECURSE spikeeditor_header_files "src/*.h")

add_executable(SpikeEditor
    ${spikeeditor_source_files}
    ${spikeeditor_header_files}
)

target_include_directories(SpikeEditor PRIVATE
    "src"
    "Spike/vendor"
    "Spike/vendor/assimp/include"
)

file(COPY ${CMAKE_SOURCE_DIR}/Spike/vendor/assimp/lib/assimp-vc142-mt.dll DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")
file(COPY ${CMAKE_SOURCE_DIR}/Spike/vendor/assimp/lib/assimp-vc142-mt.dll DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Dist")
file(COPY ${CMAKE_SOURCE_DIR}/Spike/vendor/assimp/lib/assimp-vc142-mt.dll DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release")

set_property(TARGET SpikeEditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SpikeEditor)

target_link_libraries(SpikeEditor PRIVATE Spike)