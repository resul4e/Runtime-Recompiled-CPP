project(Audio VERSION 1.0.0)

find_package(SFML 2.5 COMPONENTS audio REQUIRED)

add_library("Audio" SHARED
    "ExportDetails.h"
    "AudioComponent.h"
    "AudioComponent.cpp"
    "AudioPlugin.h"
    "AudioPlugin.cpp")

target_link_libraries("Audio" "PluginBase")
target_link_libraries("Audio" "Core")
target_link_libraries("Audio" sfml-audio)
target_compile_definitions("Audio" PRIVATE AUDIO_DLL)