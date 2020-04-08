project(Main VERSION 1.0.0)

add_executable("Main" "main.cpp")
target_link_libraries("Main" "EngineBase")
target_link_libraries("Main" "Core")
target_link_libraries("Main" "PluginCore")
target_link_libraries("Main" "Rendering")
target_link_libraries("Main" "Input")