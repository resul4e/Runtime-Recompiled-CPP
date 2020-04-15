project(Physics VERSION 1.0.0)

add_library("Physics" SHARED
    "ExportDetails.h"
    "dllmain.cpp"
    "ContactListener.h"
    "ContactListener.cpp"
    "PhysicsComponent.h"
    "PhysicsComponent.cpp"
    "PhysicsPlugin.h"
    "PhysicsPlugin.cpp"
    "stdafx.h"
    "stdafx.cpp"
    "targetver.h")
set_target_properties("Physics" PROPERTIES FOLDER Plugins)
#find_package(Box2D REQUIRED)

target_link_libraries("Physics" "PluginBase")
target_link_libraries("Physics" "Core")
target_include_directories("Physics" PRIVATE "${CMAKE_SOURCE_DIR}/Engine/Extern")
target_link_libraries("Physics" "${CMAKE_SOURCE_DIR}/Engine/Extern/Box2D/bin/Release/Box2D.lib")
target_compile_definitions("Physics" PRIVATE PHYSICS_DLL)