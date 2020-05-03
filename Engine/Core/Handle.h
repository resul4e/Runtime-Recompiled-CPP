#pragma once
#include "SizeT.h"
#include "ExportDetails.h"

class Level;

/**
 * \brief Handle that you get returned when you create a new Object.
 */
struct CORE_API ObjectHandle
{
	size_t index;
};

/**
* \brief Handle that you get returned when you create a new RenderComponent.
*/
struct CORE_API RenderCompHandle
{
	size_t index;
};

/**
* \brief Handle that you get returned when you create a new SpriteComponent.
*/
struct CORE_API SpriteCompHandle
{
	size_t spriteIndex;
	const char* textureIndex;
};

/**
* \brief Handle that you get returned when you create a new AnimationComponent.
*/
struct CORE_API AnimationCompHandle
{
	size_t spriteIndex;
	char currentAnimation[512];
};

/**
* \brief Handle that you get returned when you create a new InputComponent.
*/
struct CORE_API InputCompHandle
{
	size_t index;
};

/**
* \brief Handle that you get returned when you create a new Logger.
*/
struct CORE_API LoggerHandle
{
	const char* name;
};

/**
* \brief Handle that you get returned when you create a new AudioComponent.
*/
struct CORE_API AudioCompHandle
{
	size_t index;
	const char* bufferIndex;
};

/**
* \brief Handle that you get returned when you create a new PhysicsComponent.
*/
struct CORE_API PhysicsCompHandle
{
	size_t index;
};