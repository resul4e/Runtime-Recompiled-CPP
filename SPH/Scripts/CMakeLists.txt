set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

project("SPH")

add_library("SPH" SHARED 
"Game.cpp"
"Game.h"
"Particle.cpp"
"Particle.h"
)

target_link_libraries("SPH" "Core")
target_link_libraries("SPH" "Transform")
target_link_libraries("SPH" "Rendering")
target_link_libraries("SPH" "Physics")
target_include_directories("SPH" PRIVATE "${CMAKE_SOURCE_DIR}/Engine")
target_include_directories("SPH" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Plugins")
target_include_directories("SPH" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Extern/spdlog/include")
target_include_directories("SPH" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Extern/glm")
target_include_directories("SPH" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Extern")