project(Main VERSION 1.0.0)

add_executable("Main" "main.cpp")
target_link_libraries("Main" "EngineBase")
target_link_libraries("Main" "Core")
target_link_libraries("Main" "PluginCore")
target_link_libraries("Main" "Rendering")
target_link_libraries("Main" "Input")

#set the source dir for use in the engine.
target_compile_definitions("Main" PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")

#copy the sfml dlls from the extern folder to the output folder
add_custom_command(TARGET Main POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/Engine/Extern/SFML/bin" #from
    "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$(ConfigurationName)") #to

#create a debug command line option to poin to the correct path.
FILE(WRITE "${CMAKE_CURRENT_BINARY_DIR}/Main.vcxproj.user"
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<Project ToolsVersion=\"Current\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\n"
"    <LocalDebuggerCommandArguments>${CMAKE_BINARY_DIR}/${GAME_NAME}</LocalDebuggerCommandArguments>\n"
"    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n"
"  </PropertyGroup>\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='MinSizeRel|x64'\">\n"
"    <LocalDebuggerCommandArguments>${CMAKE_BINARY_DIR}/${GAME_NAME}</LocalDebuggerCommandArguments>\n"
"    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n"
"  </PropertyGroup>\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">\n"
"    <LocalDebuggerCommandArguments>${CMAKE_BINARY_DIR}/${GAME_NAME}</LocalDebuggerCommandArguments>\n"
"    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n"
"  </PropertyGroup>\n"
"  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='RelWithDebInfo|x64'\">\n"
"    <LocalDebuggerCommandArguments>${CMAKE_BINARY_DIR}/${GAME_NAME}</LocalDebuggerCommandArguments>\n"
"    <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>\n"
"  </PropertyGroup>\n"
"</Project>\n")