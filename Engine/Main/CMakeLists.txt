project(Main VERSION 1.0.0)

add_executable("Main" "main.cpp")
target_link_libraries("Main" "EngineBase")
target_link_libraries("Main" "Core")
target_link_libraries("Main" "PluginCore")
target_link_libraries("Main" "Rendering")
target_link_libraries("Main" "Input")

target_compile_definitions("Main" PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_custom_command(TARGET Main POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/Engine/Extern/SFML/bin" #from
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$(ConfigurationName)") #to