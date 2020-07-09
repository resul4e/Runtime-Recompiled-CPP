project(Main VERSION 1.0.0)

add_executable("Main" 
    "main.cpp")

target_link_libraries("Main" "EngineBase")
target_link_libraries("Main" "Core")
target_link_libraries("Main" "PluginCore")
target_link_libraries("Main" "Rendering")
target_link_libraries("Main" "Input")
target_link_libraries("Main" sfml-graphics)
target_link_libraries("Main" sfml-window)

#set the source dir for use in the engine.
target_compile_definitions("Main" PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")
target_compile_definitions("Main" PRIVATE BINARY_DIR="${CMAKE_BINARY_DIR}")
target_compile_definitions("Main" PRIVATE GAME_NAME="${GAME_NAME}")

if(WIN32)
    #copy the sfml dlls from the extern folder to the output folder
    add_custom_command(TARGET Main POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_SOURCE_DIR}/Engine/Extern/SFML/bin" #from
        "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$(ConfigurationName)") #to

    #copy the vswhere tools from the source folder to the ouput folder
    add_custom_command(TARGET Main POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_SOURCE_DIR}/Engine/Extern/VSWhere" #from
        "${CMAKE_BINARY_DIR}/Engine/Extern/VSWhere") #to
endif()