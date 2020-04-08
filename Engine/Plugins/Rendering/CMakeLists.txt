project(Rendering VERSION 1.0.0)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_library("Rendering" SHARED
    "ExportDetails.h"
    "RenderEngineAPI.h"
    "RenderEngineAPI.cpp"
    "RenderComponent.h"
    "RenderComponent.cpp")

target_link_libraries("Rendering" "Base")
target_link_libraries("Main" sfml-graphics)
target_compile_definitions("Rendering" PRIVATE RENDERING_DLL PRIVATE ENGINE_PROJECT)