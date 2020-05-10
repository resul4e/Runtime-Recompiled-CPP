#pragma once
#include "ExportDetails.h"

namespace sf
{
	class Window;
}

/**
 * \brief Used to interface with the Renderer in the Engine.
 */
class RENDERING_API RenderEngineAPI
{
	RenderEngineAPI(){};
	~RenderEngineAPI(){};
public:

private:
	friend class Window;
	friend class RenderComponent;
	friend class InputPlugin;
	friend class AnimationComponent;
	friend int main(int argc, char* args[]);

	static sf::Window* GetRenderWindow() { return window; };
	static bool GetIsWindowClosed() { return isWindowClosed; };
	static void SetIsWindowClosed() { isWindowClosed = true; };
public:
private:
	static bool isWindowClosed;
	static sf::Window* window;			///has to be a raw pointer, because it is going through shared library boundary
};
