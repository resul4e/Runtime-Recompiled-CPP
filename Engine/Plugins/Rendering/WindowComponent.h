#pragma once
#include "ExportDetails.h"

#include <memory>

#include "Transform/Vector.h"


namespace sf
{
	class RenderWindow;
	class View;
}

/**
 * \brief Class that changes the window and its view.
 */
class WindowComponent
{
	WindowComponent();
	~WindowComponent();
public:

	/**
	 * \brief Sets the view to a new position.
	 * \param aNewPos The new position of the view
	 */
	RENDERING_API static void SetViewPosition(glm::vec2 aNewPos);
	/**
	 * \brief Moves the view to a new position by adding the offset to the old position.
	 * \param aOffset The offset from the current view position
	 */
	RENDERING_API static void MoveViewPosition(glm::vec2 aOffset);
	/**
	 * \brief Returns the current view position.
	 * \return the current view position.
	 */
	RENDERING_API static glm::vec2 GetViewPosition();
	/**
	 * \brief Changes the title of the window.
	 * \param title The new title of the window.
	 */
	RENDERING_API static void ChangeWindowTitle(const char* title);
	/**
	 * \brief turns the verticalSync on or off.
	 * \param isVsyncEnabled If vSync is enabled or not.
	 */
	RENDERING_API static void SetVerticalSync(bool isVsyncEnabled = true);
	RENDERING_API static void SetBackgroundColor(glm::vec4 aColor);
private:

public:

private:
	friend class Window;

	///The implementation specific RenderWindow, at the moment it is SFML.
	static std::shared_ptr<sf::RenderWindow> window;
	///The implementation specific View, at the moment it is SFML.
	static std::shared_ptr<sf::View> view;
	///The color of the background.
	static glm::vec4 clearColor;
};

