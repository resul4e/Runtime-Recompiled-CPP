cmake_minimum_required(VERSION 3.16)

project(Project)

set("GAME_DIRECTORY" CACHE FILEPATH "Filepath to the game directory with a cmakelists.txt file.")

message("${GAME_DIRECTORY}")

if(NOT "${GAME_DIRECTORY}" STREQUAL "")
    add_subdirectory("${GAME_DIRECTORY}")
else()
    message("No game directory included, only building the engine.")
endif()

add_subdirectory("Engine")
