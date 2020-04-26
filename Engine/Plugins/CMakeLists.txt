add_library("PluginBase" INTERFACE)
target_link_libraries("PluginBase" INTERFACE "EngineBase")
target_link_libraries("PluginBase" INTERFACE "PluginCore")

add_subdirectory(Transform)
add_subdirectory(Rendering)
add_subdirectory(Input)
add_subdirectory(Physics)
add_subdirectory(Audio)
add_subdirectory(TestPlugin)

