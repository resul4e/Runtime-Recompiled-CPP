project(Rendering VERSION 1.0.0)

add_library("Rendering" SHARED
    "ExportDetails.h"
    "RenderEngineAPI.h"
    "RenderEngineAPI.cpp"
    "RenderComponent.h"
    "RenderComponent.cpp"
    "WindowComponent.h"
    "WindowComponent.cpp"
    "RenderPlugin.h"
    "RenderPlugin.cpp"
    "Window.h"
    "Window.cpp"
    "AnimatedSprite.h"
    "AnimatedSprite.cpp"
    "SpriteComponent.h"
    "SpriteComponent.cpp"
    "Animation.h"
    "Animation.cpp"
    "AnimationComponent.h"
    "AnimationComponent.cpp"
    "Enums.h")

set_target_properties("Rendering" PROPERTIES FOLDER Plugins)

target_link_libraries("Rendering" "PluginBase")
target_link_libraries("Rendering" "Core")
target_link_libraries("Rendering" "PIL")
target_link_libraries("Rendering" sfml-graphics)
target_link_libraries("Rendering" sfml-window)
target_compile_definitions("Rendering" PRIVATE RENDERING_DLL)