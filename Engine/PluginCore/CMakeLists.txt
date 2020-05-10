project(PluginCore VERSION 1.0.0)

add_library("PluginCore" STATIC
    "ExportDetails.h"
    "PluginBase.h"
    "PluginBase.cpp"
    "PluginLoader.h"
    "PluginLoader.cpp"
)
target_link_libraries("PluginCore" "EngineBase")
target_link_libraries("PluginCore" "PIL")
target_include_directories("PluginCore" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/PIL")
target_compile_definitions("PluginCore" PRIVATE PLUGIN_DLL PRIVATE ENGINE_PROJECT)
target_compile_definitions("PluginCore" PRIVATE OUTPUT_DIRECTORY="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/")