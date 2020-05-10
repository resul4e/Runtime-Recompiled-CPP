#pragma once
#include "ExportDetails.h"

#include <memory>
#include "Transform/Vector.h"
#include <unordered_map>
#include <vector>

//forward declaration
struct ConfigDirectories;
struct AnimationCompHandle;
class Animation;
class AnimatedSprite;

/**
 * \brief Component that is able to animate sprites
 * 
 * You can add multiple animations to one sprite, each animation consists of a certain amount of frames.
 * These frames have to be manually added.
 * 
 * Usage:
 * 
 * There are a few steps to show a sprite on the screen
 * Firstly you add a new sprite using AddSprite.
 * Then add a new animation with AddAnimation.
 * After this you need to at least add one animation fram using AddAnimationFrame.
 * Lastly you can Set the animation to be played using SetAnimation.
 * 
 * \code{.cpp}
 * void Start()
 * {
 *		spriteHandle = AddSprite();
 *		AddAnimation("ANIMATIONNAME", "PATH_TO_SPRITE_SHEET");
 *		AddAnimationFrame("SAME_ANIMATIONNAME_AS_ABOVE", [START_OF_RECT], [END_OF_RECT])
 *		.
 *		.
 *		.
 *		SetAnimation(spriteHandle, "SAME_ANIMATIONNAME_AS_ABOVE");
 * }
 * 
 * void Update()
 * {
 *		if(changeAnimation)
 *		{
 *			SetAnimation(spriteHandle, "ANOTHER_ANIMATIONNAME");
 *		}
 * }
 * \endcode
 * 
 * The animation can be stopped, paused and played.
 * 
 * Transparency can be changed by changing the alpha of the color (keeping the other values 255).
 */
class AnimationComponent
{
	AnimationComponent() {};
	~AnimationComponent() {};
public:
	/**
	 * \brief Adds a new sprite. 
	 * 
	 * \note This alone will show nothing. To have something on the screen 
	 * you need at least an animation with at least one frame.
	 * 
	 * \param aFrameTime How long each frame will be shown for in milliseconds.
	 * \param aStopped If the sprite animation should not start immediately.
	 * \param aLooped If the animation should loop
	 * \return An AnimationComponent
	 */
	RENDERING_API static AnimationCompHandle AddSprite(int aFrameTime = 256, bool aStopped = false, bool aLooped = true);
	/**
	 * \brief Adds an animation with a specified name and opens a spriteSheet.
	 * \param aAnimationName The identifier of the Animation.
	 * \param aSpriteSheet The sprite sheet of the Animation. All frames of the animation will be taken from this spriteSheet.
	 */
	RENDERING_API static void AddAnimation(const char* aAnimationName, const char* aSpriteSheet);
	/**
	 * \brief Adds a frame to the specified animation.
	 * \param aAnimationName The identifier of the Animation you want to add a frame to.
	 * \param aRectStart The top-left corner of the frame.
	 * \param aRectDimensions The width and height of the frame.
	 */
	RENDERING_API static void AddAnimationFrame(const char* aAnimationName, glm::ivec2 aRectStart, glm::ivec2 aRectDimensions);
	/**
	 * \brief Sets the currect Animation to be played.
	 * \param aHandle The sprite you want to change.
	 * \param aAnimationName The Animation you want to play.
	 */
	RENDERING_API static void SetAnimation(AnimationCompHandle& aHandle, const char* aAnimationName);
	RENDERING_API static const char* GetCurrentAnimation(AnimationCompHandle aHandle);

	/**
	 * \brief Plays the current Animation on the specified Sprite.
	 * \param aHandle The Sprite you want to play.
	 */
	RENDERING_API static void Play(AnimationCompHandle aHandle);
	/**
	 * \brief Pauses the Animation on the specified Sprite.
	 * \param aHandle The Sprite you want to pause.
	 * \note Pausing will not reset the internal frameTimer in contrast to Stop which will.
	 * \see Play()
	 * \see Stop()
	 */
	RENDERING_API static void Pause(AnimationCompHandle aHandle);
	/**
	 * \brief Stops the Animation on the specied Srite.
	 * \param aHandle The Sprite you wan to stop.
	 * \note Stopping will reset the internal frameTimer in contrast to Pause which will not.
	 * \see Pause()
	 * \see Play()
	 */
	RENDERING_API static void Stop(AnimationCompHandle aHandle);
	
	/**
	 * \brief Gets how long each frame will be shown for.
	 * \param aHandle The Sprite you want the information from.
	 * \return The frametime in milliseconds.
	 */
	RENDERING_API static int GetFrameTime(AnimationCompHandle aHandle);
	/**
	 * \brief Sets a new FramTime in milliseconds.
	 * \param aHandle The Sprite you want to change.
	 * \param aNewFrameTime How long each fram will be shown for in milliseconds.
	 */
	RENDERING_API static void SetFrameTime(AnimationCompHandle aHandle, int aNewFrameTime);

	/**
	 * \brief Sets a new color for the sprite.
	 * \param aHandle The Sprite you want to change the color of.
	 * \param aNewColor The new color.
	 * \note To make the sprite transparent, change the alpha while keeping the other values 255.
	 */
	RENDERING_API static void SetColor(AnimationCompHandle aHandle, glm::tvec4<char> aNewColor);

	/**
	 * \brief Gets the position of the Sprite
	 * \param aHandle The Sprite you want the position of.
	 * \return The position of the Sprite.
	 */
	RENDERING_API static glm::vec2 GetPosition(AnimationCompHandle aHandle);
	/**
	 * \brief Sets a new position for a Sprite.
	 * \param aHandle The Sprite you want to change the position of.
	 * \param aNewPostion The new position.
	 */
	RENDERING_API static void SetPosition(AnimationCompHandle aHandle, glm::vec2 aNewPostion);

	/**
	 * \brief Gets the origin of the Sprite.
	 * \param aHandle The Sprite you want the origin of.
	 * \return The origin.
	 */
	RENDERING_API static glm::vec2 GetOrigin(AnimationCompHandle aHandle);
	/**
	 * \brief Sets the Origin of the Sprite.
	 * \param aHandle The Sprite you want to change the origin of.
	 * \param aNewOrigin The new origin.
	 */
	RENDERING_API static void SetOrigin(AnimationCompHandle aHandle, glm::vec2 aNewOrigin);

	/**
	 * \brief Gets the scale of the Sprite.
	 * \param aHandle The Sprite you want the scale of.
	 * \return The current scale of the Sprite.
	 */
	RENDERING_API static glm::vec2 GetScale(AnimationCompHandle aHandle);
	/**
	 * \brief Sets a new scale for the specified sprite.
	 * \param aHandle The Sprite to change.
	 * \param aNewScale The new scale of the Sprite.
	 * \note making the scale (-1,1) will flip the sprite on the x-axis.
	 */
	RENDERING_API static void SetScale(AnimationCompHandle aHandle, glm::vec2 aNewScale);
	RENDERING_API static glm::ivec2 GetScreenPosition(AnimationCompHandle aHandle);
	RENDERING_API static void RemoveSprite(AnimationCompHandle& aHandle);
private:
public:
private:
	friend class Window;
	friend class RenderPlugin;

	///A list of all of the animations. These can be used on any of the AnimationSprites.
	static std::unordered_map<std::string, std::shared_ptr<Animation>> animationList;
	///A list of all of the AnimationSprites. 
	static std::vector<std::shared_ptr<AnimatedSprite>> animatedSpriteList;

	///
	static std::shared_ptr<ConfigDirectories> directories;
};