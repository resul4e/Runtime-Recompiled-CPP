#pragma once
#include <memory>

namespace sf
{
	class RenderWindow;
}

/**
 * \brief Creates and Updates the SFML window
 */
class Window
{
public:
	Window();
	~Window();
	/**
	 * \brief Creates the RenderWindow.
	 * 
	 * Also sets VSync.
	 */
	void Start();
	/**
	 * \brief Displays all of the RenderComponents.
	 */
	void Update(float aDeltaTime);
	/**
	 * \brief Clears all of the lists in the various components in this plugin.
	 */
	void Delete();
private:

//variables
public:
private:

	///The implementation specific RenderWindow, at the moment it is SFML
	std::shared_ptr<sf::RenderWindow> renderWindow;
};

