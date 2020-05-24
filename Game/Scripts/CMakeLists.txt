set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

project("Game")

add_library("Game" SHARED 
"Game.cpp"
"Game.h"
"Background.cpp"
"Background.h"
"Block.cpp"
"Block.h"
"BrickBlock.cpp"
"BrickBlock.h"
"CoinBlock.cpp"
"CoinBlock.h"
"Enemy.cpp"
"Enemy.h"
"Goomba.cpp"
"Goomba.h"
"Mario.cpp"
"Mario.h")

target_link_libraries("Game" "Core")
target_link_libraries("Game" "Transform")
target_link_libraries("Game" "Rendering")
target_link_libraries("Game" "Physics")
target_link_libraries("Game" "Audio")
target_link_libraries("Game" "Input")
target_include_directories("Game" PRIVATE "${CMAKE_SOURCE_DIR}/Engine")
target_include_directories("Game" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Plugins")
target_include_directories("Game" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Extern/spdlog/include")
target_include_directories("Game" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Extern/glm")
target_include_directories("Game" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Extern")
target_include_directories("Game" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/PIL")
target_compile_definitions("Game" PRIVATE _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING)