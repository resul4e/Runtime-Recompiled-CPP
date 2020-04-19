#include "AnimationComponent.h"

#include "SpriteComponent.h"

#include "SFML/Graphics.hpp"
#include "AnimatedSprite.h"
#include "Animation.h"

#include "ConfigDirectories.h"
#include "Handle.h"
#include "Logger.h"
#include "RenderEngineAPI.h"

std::unordered_map<std::string, std::shared_ptr<Animation>> AnimationComponent::animationList;
std::vector<std::shared_ptr<AnimatedSprite>> AnimationComponent::animatedSpriteList;
std::shared_ptr<ConfigDirectories> AnimationComponent::directories;

extern "C" RENDERING_API AnimationCompHandle AnimationComponent::AddSprite(int aFrameTime, bool aStopped, bool aLooped)
{
	animatedSpriteList.push_back(std::make_shared<AnimatedSprite>(sf::milliseconds(aFrameTime), aStopped, aLooped));

	return { animatedSpriteList.size() - 1 };
}

extern "C" RENDERING_API void AnimationComponent::AddAnimation(const char* aAnimationName, const char* aSpriteSheet)
{
	//check if the texture is already added, if so use that one, otherwise create a new texture.
	std::shared_ptr<sf::Texture> tempTexture;
	if (SpriteComponent::textureList.find(aSpriteSheet) == SpriteComponent::textureList.end())
	{
		std::string filePath = (directories->RootGameSourceDirectory / aSpriteSheet).string();
		
		tempTexture = std::make_shared<sf::Texture>();
		tempTexture->loadFromFile(filePath);
		SpriteComponent::textureList.insert(std::pair<std::string, std::shared_ptr<sf::Texture>>(aSpriteSheet, tempTexture));
	}
	else
	{
		tempTexture = SpriteComponent::textureList.at(aSpriteSheet);
	}

	animationList.insert(std::pair<std::string, std::shared_ptr<Animation>>(aAnimationName, std::make_shared<Animation>()));
	animationList.at(aAnimationName)->setSpriteSheet(*tempTexture);
}

extern "C" RENDERING_API void AnimationComponent::AddAnimationFrame(const char* aAnimationName,
	glm::ivec2 aRectStart, glm::ivec2 aRectDimensions)
{
	if (animationList.find(aAnimationName) == animationList.end())
	{
		LOG_ERROR(Logger::Get("Rendering"), "No animation with the name {}", aAnimationName);
		return;
	}

	animationList.at(aAnimationName)->addFrame(sf::IntRect(aRectStart.x, aRectStart.y, aRectDimensions.r, aRectDimensions.g));
}

extern "C" RENDERING_API void AnimationComponent::SetAnimation(AnimationCompHandle& aHandle, const char* aAnimationName)
{
	if (animationList.find(aAnimationName) == animationList.end())
	{
		LOG_ERROR(Logger::Get("Rendering"), "No animation with the name {}", aAnimationName);
		return;
	}

	strcpy_s(aHandle.currentAnimation, 512, aAnimationName);
	animatedSpriteList.at(aHandle.spriteIndex)->setAnimation(*animationList.at(aAnimationName));
}

extern "C" RENDERING_API const char* AnimationComponent::GetCurrentAnimation(AnimationCompHandle aHandle)
{
	if(aHandle.currentAnimation == nullptr)
	{
		return "THE_ANIMATION_IS_NOTHING_._THIS_STRING_IS_VERY_LONG_TO_MINIMIZE_THE_CHANCE_THAT_AN_ANIMATION_HAS_THE_SAME_NAME";
	}
	return aHandle.currentAnimation;
}

extern "C" RENDERING_API void AnimationComponent::Play(AnimationCompHandle aHandle)
{
	animatedSpriteList.at(aHandle.spriteIndex)->play();
}

extern "C" RENDERING_API void AnimationComponent::Pause(AnimationCompHandle aHandle)
{
	animatedSpriteList.at(aHandle.spriteIndex)->pause();
}

extern "C" RENDERING_API void AnimationComponent::Stop(AnimationCompHandle aHandle)
{
	animatedSpriteList.at(aHandle.spriteIndex)->stop();
}

extern "C" RENDERING_API int AnimationComponent::GetFrameTime(AnimationCompHandle aHandle)
{
	return animatedSpriteList.at(aHandle.spriteIndex)->getFrameTime().asMilliseconds();
}

extern "C" RENDERING_API void AnimationComponent::SetFrameTime(AnimationCompHandle aHandle, int aNewFrameTime)
{
	animatedSpriteList.at(aHandle.spriteIndex)->setFrameTime(sf::milliseconds(aNewFrameTime));
}

extern "C" RENDERING_API void AnimationComponent::SetColor(AnimationCompHandle aHandle, glm::tvec4<char> aNewColor)
{
	animatedSpriteList.at(aHandle.spriteIndex)->setColor(sf::Color(aNewColor.x, aNewColor.y, aNewColor.z, aNewColor.w));
}

glm::vec2 AnimationComponent::GetPosition(AnimationCompHandle aHandle)
{
	const sf::Vector2f position = animatedSpriteList.at(aHandle.spriteIndex)->getPosition();
	return { position.x, position.y };
}

extern "C" RENDERING_API void AnimationComponent::SetPosition(AnimationCompHandle aHandle, glm::vec2 aNewPostion)
{
	animatedSpriteList.at(aHandle.spriteIndex)->setPosition(aNewPostion.x, aNewPostion.y);
}

extern "C" RENDERING_API glm::vec2 AnimationComponent::GetOrigin(AnimationCompHandle aHandle)
{
	const sf::Vector2f origin = animatedSpriteList.at(aHandle.spriteIndex)->getOrigin();
	return glm::vec2(origin.x, origin.y);
}

extern "C" RENDERING_API void AnimationComponent::SetOrigin(AnimationCompHandle aHandle, glm::vec2 aNewOrigin)
{
	animatedSpriteList.at(aHandle.spriteIndex)->setOrigin(aNewOrigin.x, aNewOrigin.y);
}

extern "C" RENDERING_API glm::vec2 AnimationComponent::GetScale(AnimationCompHandle aHandle)
{
	const sf::Vector2f scale = animatedSpriteList.at(aHandle.spriteIndex)->getScale();
	return glm::vec2(scale.x, scale.y);
}

extern "C" RENDERING_API void AnimationComponent::SetScale(AnimationCompHandle aHandle, glm::vec2 aNewScale)
{
	animatedSpriteList.at(aHandle.spriteIndex)->setScale(aNewScale.x, aNewScale.y);
}

glm::ivec2 AnimationComponent::GetScreenPosition(AnimationCompHandle aHandle)
{
	sf::RenderWindow* window = static_cast<sf::RenderWindow*>(RenderEngineAPI::GetRenderWindow());
	sf::Vector2i pos = window->mapCoordsToPixel(animatedSpriteList.at(aHandle.spriteIndex)->getPosition());
	return { pos.x,pos.y };
}

void AnimationComponent::RemoveSprite(AnimationCompHandle& aHandle)
{
	animatedSpriteList.at(aHandle.spriteIndex).reset();
	aHandle = { 1231234, "NOTHING" };
}
