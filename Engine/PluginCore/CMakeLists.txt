project(PluginCore VERSION 1.0.0)

add_library("PluginCore" STATIC
    "ExportDetails.h"
    "PluginBase.h"
    "PluginBase.cpp"
    "PluginLoader.h"
    "PluginLoader.cpp"
)
target_link_libraries("PluginCore" "EngineBase")
target_compile_definitions("PluginCore" PRIVATE PLUGIN_DLL PRIVATE ENGINE_PROJECT)
target_compile_definitions("PluginCore" PRIVATE TARGET_DIRECTORY="${CMAKE_BINARY_DIR}")