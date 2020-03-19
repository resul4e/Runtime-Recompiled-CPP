#include "RenderEngineAPI.h"
#include "SFML/Graphics.hpp"

#ifdef ENGINE_PROJECT
bool RenderEngineAPI::isWindowClosed = false;
sf::Window* RenderEngineAPI::window;
#endif