file(GLOB_RECURSE spikescriptcore_source_files "src/*.cs")
project("Spike-ScriptCore" CSharp)

add_library(Spike-ScriptCore SHARED
    ${spikescriptcore_source_files}
)
