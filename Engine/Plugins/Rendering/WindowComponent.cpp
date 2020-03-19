#include "WindowComponent.h"

#include "SFML/Graphics.hpp"

std::shared_ptr<sf::RenderWindow> WindowComponent::window;
std::shared_ptr < sf::View> WindowComponent::view;
glm::vec4 WindowComponent::clearColor;

WindowComponent::WindowComponent()
{
}


WindowComponent::~WindowComponent()
{
}

extern "C" RENDERING_API void WindowComponent::SetViewPosition(glm::vec2 aNewPos)
{
	view->setCenter(aNewPos.x, aNewPos.y);
	window->setView(*view);
}

glm::vec2 WindowComponent::GetViewPosition()
{
	sf::Vector2f pos = view->getCenter();
	return { pos.x,pos.y };
}

void WindowComponent::MoveViewPosition(glm::vec2 aOffset)
{
	view->move(aOffset.x, aOffset.y);
	window->setView(*view);
}

void WindowComponent::ChangeWindowTitle(const char* title)
{
	window->setTitle(sf::String(title));
}

void WindowComponent::SetVerticalSync(bool isVsyncEnabled)
{
	window->setVerticalSyncEnabled(isVsyncEnabled);
}

void WindowComponent::SetBackgroundColor(glm::vec4 aColor)
{
	clearColor = aColor;
}
