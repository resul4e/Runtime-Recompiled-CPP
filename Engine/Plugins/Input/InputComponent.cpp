#include "InputComponent.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window.hpp>
#include "Handle.h"

sf::Window* InputComponent::window;
glm::ivec2 InputComponent::mousepos = glm::ivec2(0,0);


InputComponent::InputComponent()
{
}


InputComponent::~InputComponent()
{
}

extern "C" INPUT_API bool InputComponent::GetKeyPressed(Key aKeyCode)
{
	if(window->hasFocus())
	{
		return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(aKeyCode));
	}
	return false;
}

extern "C" INPUT_API bool InputComponent::GetMouseButtonPressed(MouseButton aMouseButton)
{
	if (window->hasFocus())
	{
		return sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(aMouseButton));
	}
	return false;
}

extern "C" INPUT_API glm::ivec2 InputComponent::GetMousePosition()
{
	const sf::Vector2i tempVec = sf::Mouse::getPosition(*window);
	mousepos = glm::ivec2(tempVec.x, tempVec.y);
	return mousepos;
}
