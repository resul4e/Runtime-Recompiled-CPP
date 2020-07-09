project("Plugins")

add_library("PluginBase" INTERFACE)
target_link_libraries("PluginBase" INTERFACE "EngineBase")
target_link_libraries("PluginBase" INTERFACE "PluginCore")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Plugins")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Plugins")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Plugins")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Plugins")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Plugins")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Plugins")

add_subdirectory(Transform)
add_subdirectory(Rendering)
add_subdirectory(Input)
add_subdirectory(Physics)
add_subdirectory(Audio)
add_subdirectory(TestPlugin)

