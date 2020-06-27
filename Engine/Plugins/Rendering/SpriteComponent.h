#pragma once
#include "ExportDetails.h"

#include "FileSystem.h"
#include "Transform/Vector.h"
#include <unordered_map>
#include <memory>
#include <vector>

//forward declare
struct SpriteCompHandle;
struct AnimationCompHandle;
struct ConfigDirectories;
class Animation;
class AnimatedSprite;

namespace sf
{
	class Sprite;
	class Texture;
}

/**
 * \brief Class that shows static sprites on the screen.
 * For animations see the AnimationComponent.
 */
class SpriteComponent
{
	SpriteComponent(){};
	~SpriteComponent(){};
public:
	/**
	* \brief Creates a SpriteComponent.
	* \param aImage 
	* \see Add()
	* \see RenderShape
	* \return A handle to the SpriteComponent.
	*/
	/**
	 * \brief Adds a sprite to the scene. RectPos and RectSize 
	 * can be used to extract a certain sprite from a spritesheet.
	 * \param aImage	The path to the image.
	 * \param rectPos	The top-left corner of the sprite.
	 * \param rectSize	The width and height of the sprite.
	 * \return A new spriteComponent.
	 */
	RENDERING_API static SpriteCompHandle AddSprite(const char* aImage, 
		glm::ivec2 rectPos = glm::ivec2(0,0), glm::ivec2 rectSize = glm::ivec2(0,0));
	
	/**
	 * \brief Sets a new position for the sprite.
	 * \param aHandle The sprite to move.
	 * \param aPosition The new position of the sprite.
	 */
	RENDERING_API static void SetPosition(SpriteCompHandle aHandle, glm::vec2 aPosition);
	/**
	 * \brief Gets the position of the sprite.
	 * \param aHandle The Sprite of which you want the position
	 * \return The position of the Sprite.
	 */
	RENDERING_API static glm::vec2 GetPosition(SpriteCompHandle aHandle);

	/**
	 * \brief Sets a new origin for the Sprite.
	 * \param aHandle The Sprite you want to change the origin of.
	 * \param aOrigin The new position of the origin.
	 */
	RENDERING_API static void SetOrigin(SpriteCompHandle aHandle, glm::vec2 aOrigin);
	/**
	 * \brief Gets the origin of the sprite.
	 * \param aHandle The Sprite you want to get the origin of.
	 * \return The Origin of the Sprite.
	 */
	RENDERING_API static glm::vec2 GetOrigin(SpriteCompHandle aHandle);

	/**
	 * \brief Sets a new texture for the Sprite.
	 * \param aHandle The Sprite you want to change the Texture of.
	 * \param aImage The new texture to apply to the Sprite.
	 * \param resetRect if you want to reset the rect dimensions.	
	 */
	RENDERING_API static void SetTexture(SpriteCompHandle& aHandle, const char* aImage, bool resetRect = false);
	/**
	 * \brief Sets a new TextureRect for the Sprite.
	 * \param aHandle The Sprite you want to change the spriterect of.
	 * \param rectPos The top-left corner of the Rect.
	 * \param rectSize The width and height of the Rect.
	 */
	RENDERING_API static void SetTextureRect(SpriteCompHandle aHandle, glm::ivec2 rectPos, glm::ivec2 rectSize);
	/**
	 * \brief Gets the TextureRect.
	 * \param aHandle The Sprite you wan the TextureRect of.
	 * \return The TextureRect in the form: top, left, width, height.
	 */
	RENDERING_API static glm::ivec4 GetTextureRect(SpriteCompHandle aHandle);

	/**
	 * \brief Change the Color of the Sprite.
	 * \param aHandle The Sprite of which you want to change the Color.
	 * \param aNewColor The new Color of the Sprite.
	 */
	RENDERING_API static void SetColor(SpriteCompHandle aHandle, glm::tvec4<char> aNewColor);
	/**
	 * \brief Gets the Color of the Sprite.
	 * \param aHandle The Sprite you want the color of.
	 * \return The Color of the Sprite in the form of: r,g,b,a.
	 */
	RENDERING_API static glm::tvec4<char> GetColor(SpriteCompHandle aHandle);

	/**
	 * \brief Sets a new Scale for the Sprite.
	 * \param aHandle The Sprite you want the color of.
	 * \param aNewScale The new scale of the Sprite.
	 */
	RENDERING_API static void SetScale(SpriteCompHandle aHandle, glm::vec2 aNewScale);
	/**
	 * \brief Gets the Scale of the Sprite.
	 * \param aHandle The Sprite you want to change.
	 * \return The Scale of the Sprite.
	 */
	RENDERING_API static glm::vec2 GetScale(SpriteCompHandle aHandle);

private:

public:
private:
	friend class Window;
	friend class RenderPlugin;
	friend class AnimationComponent;

	///The sf::Textures that are stored, there shouldn't be any duplicates if implemented correctly.
	static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textureList;
	/// The sf::Sprite that are stored.
	static std::vector<std::unique_ptr<sf::Sprite>> spriteList;

	///
	static std::shared_ptr<ConfigDirectories> directories;
};

