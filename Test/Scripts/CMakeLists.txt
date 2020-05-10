set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

project("Test")

add_library("Test" SHARED 
"Game.cpp"
"Game.h"
"Particle.cpp"
"Particle.h")

target_link_libraries("Test" "Core")
target_link_libraries("Test" "Transform")
target_link_libraries("Test" "Rendering")
target_link_libraries("Test" "Physics")
target_include_directories("Test" PRIVATE "${CMAKE_SOURCE_DIR}/Engine")
target_include_directories("Test" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Plugins")
target_include_directories("Test" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Extern/spdlog/include")
target_include_directories("Test" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Extern/glm")
target_include_directories("Test" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Extern")