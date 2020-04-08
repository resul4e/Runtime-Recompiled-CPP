project(Transform VERSION 1.0.0)

add_library("Transform" SHARED
    "ExportDetails.h"
    "TransformPlugin.h"
    "TransformPlugin.cpp"
    "Vector.h"
    "Vector.cpp")

target_link_libraries("Transform" "PluginBase")

target_compile_definitions("Transform" PRIVATE TRANSFORM_DLL)