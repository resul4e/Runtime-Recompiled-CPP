cmake_minimum_required(VERSION 3.16)

#Get the name of the game to bundle with the engine libraries.
set("GAME_NAME" CACHE FILEPATH "Filepath to the game directory with a cmakelists.txt file.")
set("GAME_DIRECTORY" "${GAME_NAME}/Scripts")

#check that we got a game name
if(NOT "${GAME_NAME}" STREQUAL "")
    #create the root project.
    project("${GAME_NAME}")

    add_subdirectory("${GAME_DIRECTORY}")
else()
    message("No game directory included, only building the engine.")
endif()

add_subdirectory("Engine")
