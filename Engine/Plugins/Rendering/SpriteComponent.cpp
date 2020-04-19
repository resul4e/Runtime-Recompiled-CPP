#include "SpriteComponent.h"

#include "SFML/Graphics.hpp"
#include "AnimatedSprite.h"
#include "Animation.h"

#include "ConfigDirectories.h"
#include "Handle.h"

std::vector<std::unique_ptr<sf::Sprite>> SpriteComponent::spriteList;
std::unordered_map<std::string, std::shared_ptr<sf::Texture>> SpriteComponent::textureList;
std::shared_ptr<ConfigDirectories> SpriteComponent::directories;

extern "C" RENDERING_API SpriteCompHandle SpriteComponent::AddSprite(const char* aImage, 
	glm::ivec2 rectPos, glm::ivec2 rectSize)
{
	//check if the texture is already added, if so use that one, otherwise create a new texture.
	std::shared_ptr<sf::Texture> tempTexture;
	if (textureList.find(aImage) == textureList.end())
	{
		std::string filePath = (directories->RootGameSourceDirectory / aImage).string();
		
		tempTexture = std::make_shared<sf::Texture>();
		//TODO(Resul): Check if file has been found.
		tempTexture->loadFromFile(filePath);
		textureList.insert(std::pair<std::string, std::shared_ptr<sf::Texture>>(aImage, tempTexture));
	}
	else
	{
		tempTexture = textureList.at(aImage);
	}

	spriteList.push_back(std::make_unique<sf::Sprite>(*tempTexture));
	spriteList.back()->setTextureRect(sf::IntRect(rectPos.x, rectPos.y, rectSize.x, rectSize.y));

	return { spriteList.size() - 1, aImage };
}

extern "C" RENDERING_API void SpriteComponent::SetPosition(SpriteCompHandle aHandle, glm::vec2 aPosition)
{
	spriteList.at(aHandle.spriteIndex)->setPosition(aPosition.x, aPosition.y);
}

extern "C" RENDERING_API glm::vec2 SpriteComponent::GetPosition(SpriteCompHandle aHandle)
{
	sf::Vector2f tempPos = spriteList.at(aHandle.spriteIndex)->getPosition();
	return glm::vec2(tempPos.x, tempPos.y);
}

void SpriteComponent::SetOrigin(SpriteCompHandle aHandle, glm::vec2 aOrigin)
{
	spriteList.at(aHandle.spriteIndex)->setOrigin(aOrigin.x, aOrigin.y);
}

extern "C" RENDERING_API glm::vec2 SpriteComponent::GetOrigin(SpriteCompHandle aHandle)
{
	sf::Vector2f tempOrigin = spriteList.at(aHandle.spriteIndex)->getOrigin();
	return glm::vec2(tempOrigin.x, tempOrigin.y);
}

extern "C" RENDERING_API void SpriteComponent::SetTexture(SpriteCompHandle& aHandle, 
	const char* aImage, bool resetRect)
{
	//check if the texture is already added, if so use that one, otherwise create a new texture.
	std::shared_ptr<sf::Texture> tempTexture;
	if (textureList.find(aImage) == textureList.end())
	{
		std::string filePath = (directories->RootGameSourceDirectory / aImage).string();
		
		tempTexture = std::make_shared<sf::Texture>();
		tempTexture->loadFromFile(filePath);
		textureList.insert(std::pair<std::string, std::shared_ptr<sf::Texture>>(aImage, tempTexture));
	}
	else
	{
		tempTexture = textureList.at(aImage);
	}

	spriteList.at(aHandle.spriteIndex)->setTexture(*tempTexture, resetRect);
	aHandle.textureIndex = aImage;
}

extern "C" RENDERING_API void SpriteComponent::SetTextureRect(SpriteCompHandle aHandle, 
	glm::ivec2 rectPos, glm::ivec2 rectSize)
{
	spriteList.at(aHandle.spriteIndex)->setTextureRect(sf::IntRect(rectPos.x, rectPos.y, rectSize.x, rectSize.y));
}

glm::ivec4 stupid;
extern "C" RENDERING_API glm::ivec4 SpriteComponent::GetTextureRect(SpriteCompHandle aHandle)
{
	sf::IntRect rectDim = spriteList.at(aHandle.spriteIndex)->getTextureRect();
	stupid = glm::ivec4(rectDim.left, rectDim.top, rectDim.width, rectDim.height);
	return stupid;
	
}

extern "C" RENDERING_API void SpriteComponent::SetColor(SpriteCompHandle aHandle, glm::tvec4<char> aNewColor)
{
	spriteList.at(aHandle.spriteIndex)->setColor(sf::Color(aNewColor.x, aNewColor.y, aNewColor.z, aNewColor.w));
}

extern "C" RENDERING_API glm::tvec4<char> SpriteComponent::GetColor(SpriteCompHandle aHandle)
{
	sf::Color rectDim = spriteList.at(aHandle.spriteIndex)->getColor();
	return glm::tvec4<char>(rectDim.r, rectDim.g, rectDim.b, rectDim.a);
}

extern "C" RENDERING_API void SpriteComponent::SetScale(SpriteCompHandle aHandle, glm::vec2 aNewScale)
{
	spriteList.at(aHandle.spriteIndex)->setScale(aNewScale.x, aNewScale.y);

}

extern "C" RENDERING_API glm::vec2 SpriteComponent::GetScale(SpriteCompHandle aHandle)
{
	sf::Vector2f tempScale = spriteList.at(aHandle.spriteIndex)->getScale();
	return glm::vec2(tempScale.x, tempScale.y);
}
