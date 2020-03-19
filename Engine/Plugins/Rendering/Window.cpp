#include "Window.h"
#include "SFML/Graphics.hpp"

#include "RenderComponent.h"
#include "SpriteComponent.h"
#include "AnimationComponent.h"
#include "RenderEngineAPI.h"
#include "WindowComponent.h"
#include "AnimatedSprite.h"

#include "Transform/Vector.h"
#include "Logger.h"



Window::Window()
{
	WindowComponent::clearColor = glm::vec4(0, 0, 0, 255);
}


Window::~Window()
{
}

void Window::Start()
{
	renderWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(448, 448), "SFML works!", sf::Style::Close | sf::Style::Titlebar);
	renderWindow->setVerticalSyncEnabled(false);
	RenderEngineAPI::window = renderWindow.get();
	WindowComponent::window = renderWindow;
	WindowComponent::view = std::make_shared<sf::View>(sf::Vector2f(224, 224), sf::Vector2f(448, 448));
	renderWindow->setView(*WindowComponent::view);
}

extern std::vector<std::unique_ptr<sf::Shape>> shapeList;

void Window::Update(float aDeltatime)
{
	sf::Event event;
	
	while (renderWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			renderWindow->close();
			RenderEngineAPI::SetIsWindowClosed();
		}

		if(event.type == sf::Event::Resized)
		{
			LOG_DEBUG(Logger::Get("Rendering"), "new width: {}", event.size.width);
			LOG_DEBUG(Logger::Get("Rendering"), "new height: {}" , event.size.height);
			sf::FloatRect visibleArea(0, 0, 448, 448);
			std::shared_ptr<sf::View> newView = std::make_shared<sf::View>(visibleArea);
			newView->setViewport(sf::FloatRect(0, 0.5 - (448.f / event.size.height)/2.f, 448.f / event.size.width, 448.f / event.size.height));
			renderWindow->setView(*newView);
			WindowComponent::view = newView;
		}
	}

	renderWindow->clear(sf::Color(WindowComponent::clearColor.x, WindowComponent::clearColor.y, WindowComponent::clearColor.z, WindowComponent::clearColor.w));

	for(int i=0; i < RenderComponent::shapeList.size(); i++)
	{
		renderWindow->draw(*(RenderComponent::shapeList.at(i).get()));
	}

	for (int i = 0; i < SpriteComponent::spriteList.size(); i++)
	{
		renderWindow->draw(*(SpriteComponent::spriteList.at(i).get()));
	}

	for (int i = 0; i < AnimationComponent::animatedSpriteList.size(); i++)
	{
		if (AnimationComponent::animatedSpriteList.at(i))
		{
			AnimationComponent::animatedSpriteList.at(i)->update(sf::seconds(aDeltatime));
			renderWindow->draw(*(AnimationComponent::animatedSpriteList.at(i).get()));
		}
	}

	renderWindow->display();
}

void Window::Delete()
{
	RenderComponent::renderShapeList.clear();
	RenderComponent::shapeList.clear();

	SpriteComponent::spriteList.clear();
	SpriteComponent::textureList.clear();

	AnimationComponent::animationList.clear();
	AnimationComponent::animatedSpriteList.clear();
}
