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

#find_package(Box2D REQUIRED)

target_link_libraries("Physics" "PluginBase")
target_compile_definitions("Physics" PRIVATE PHYSICS_DLL)