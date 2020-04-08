project(Input VERSION 1.0.0)

find_package(SFML 2.5 COMPONENTS window REQUIRED)

add_library("Input" SHARED
    "ExportDetails.h"
    "InputComponent.h"
    "InputComponent.cpp"
    "InputPlugin.h"
    "InputPlugin.cpp")

target_link_libraries("Input" "PluginBase")
target_link_libraries("Input" "Rendering")
target_compile_definitions("Input" PRIVATE INPUT_DLL)