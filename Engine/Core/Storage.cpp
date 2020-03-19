#include "stdafx.h"
#include "Storage.h"
#include <cstdarg>

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SAVING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Storage::Save(const RenderCompHandle& aRenderHandle)
{
	savedRenderHandles.push(aRenderHandle);
}

void Storage::Save(const ObjectHandle& aObjectHandle)
{
	savedObjectHandles.push(aObjectHandle);
}

void Storage::Save(const float& aFloat)
{
	savedFloats.push(aFloat);
}

void Storage::Save(const glm::vec2& aVec2f)
{
	savedVec2s.push(aVec2f);
}

void Storage::Save(const bool& aBool)
{
	savedBools.push(aBool);
}

void Storage::Save(const std::string& aString)
{
	savedStrings.push(aString);
}

void Storage::Save(const AudioCompHandle& aAudioCompHandle)
{
	savedAudioHandles.push(aAudioCompHandle);
}

void Storage::Save(const SpriteCompHandle& aSpriteCompHandle)
{
	savedSpriteHandles.push(aSpriteCompHandle);
}

void Storage::Save(const AnimationCompHandle& aAnimationCompHandle)
{
	savedAnimationHandles.push(aAnimationCompHandle);
}

void Storage::Save(const PhysicsCompHandle& aPhysicsCompHandle)
{
	savedPhysicsHandles.push(aPhysicsCompHandle);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOADING
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Storage::Load(RenderCompHandle& aRenderHandle)
{
	//if you save with a certain serialization and then load with a changed one the ones you have changed will not be saved
	//so we are checking if the container is empty
	if(savedRenderHandles.empty())
	{
		aRenderHandle = RenderCompHandle();
		aRenderHandle.index = { 1231234 };
		LOG_WARN(Logger::Get("core"), "Couldn't load any more RenderComponents, because it wasn't included in the Serialize function at the last Script recompile")
		return;
	}

	aRenderHandle = savedRenderHandles.top();
	savedRenderHandles.pop();
}

void Storage::Load(ObjectHandle& aObjectHandle)
{
	//if you save with a certain serialization and then load with a changed one the ones you have changed will not be saved
	//so we are checking if the container is empty
	if (savedObjectHandles.empty())
	{
		aObjectHandle = ObjectHandle();
		aObjectHandle.index = { 1231234 };
		LOG_WARN(Logger::Get("core"), "Couldn't load any more ObjectHandles, because it wasn't included in the Serialize function at the last Script recompile")
		return;
	}

	aObjectHandle = savedObjectHandles.top();
	savedObjectHandles.pop();
}

void Storage::Load(SpriteCompHandle& aSpriteCompHandle)
{
	//if you save with a certain serialization and then load with a changed one the ones you have changed will not be saved
	//so we are checking if the container is empty
	if (savedSpriteHandles.empty())
	{
		aSpriteCompHandle = SpriteCompHandle();
		aSpriteCompHandle = { 1231234, "ERROR" };
		LOG_WARN(Logger::Get("core"), "Couldn't load any more SpriteCompHandle, because it wasn't included in the Serialize function at the last Script recompile")
			return;
	}

	aSpriteCompHandle = savedSpriteHandles.top();
	savedSpriteHandles.pop();
}

void Storage::Load(AnimationCompHandle& aAnimationCompHandle)
{
	//if you save with a certain serialization and then load with a changed one the ones you have changed will not be saved
	//so we are checking if the container is empty
	if (savedAnimationHandles.empty())
	{
		aAnimationCompHandle = AnimationCompHandle();
		aAnimationCompHandle = { 1231234 };
		LOG_WARN(Logger::Get("core"), "Couldn't load any more AnimationCompHandle, because it wasn't included in the Serialize function at the last Script recompile")
			return;
	}

	aAnimationCompHandle = savedAnimationHandles.top();
	savedAnimationHandles.pop();
}

void Storage::Load(float& aFloat)
{
	//if you save with a certain serialization and then load with a changed one the ones you have changed will not be saved
	//so we are checking if the container is empty
	if (savedFloats.empty())
	{
		aFloat = std::numeric_limits<float>::infinity();
		LOG_WARN(Logger::Get("Core"), "Couldn't load any more floats, because it wasn't included in the Serialize function at the last Script recompile")
		return;
	}

	aFloat = savedFloats.top();
	savedFloats.pop();
}

void Storage::Load(glm::vec2& aVec2f)
{
	//if you save with a certain serialization and then load with a changed one the ones you have changed will not be saved
	//so we are checking if the container is empty
	if (savedVec2s.empty())
	{
		aVec2f = glm::vec2(std::numeric_limits<float>::infinity());
		LOG_WARN(Logger::Get("core"), "Couldn't load any more vec2fs, because it wasn't included in the Serialize function at the last Script recompile")
		return;
	}

	aVec2f = savedVec2s.top();
	savedVec2s.pop();
}

void Storage::Load(AudioCompHandle& aAudioCompHandle)
{
	//if you save with a certain serialization and then load with a changed one the ones you have changed will not be saved
	//so we are checking if the container is empty
	if (savedAudioHandles.empty())
	{
		aAudioCompHandle = AudioCompHandle();
		aAudioCompHandle.index = { 1231234 };
		LOG_WARN(Logger::Get("core"), "Couldn't load any more AudioCompHandle, because it wasn't included in the Serialize function at the last Script recompile")
		return;
	}

	aAudioCompHandle = savedAudioHandles.top();
	savedAudioHandles.pop();
}

void Storage::Load(bool& aBool)
{
	//if you save with a certain serialization and then load with a changed one the ones you have changed will not be saved
	//so we are checking if the container is empty
	if (savedBools.empty())
	{
		aBool = false;
		LOG_WARN(Logger::Get("core"), "Couldn't load any more bools, because it wasn't included in the Serialize function at the last Script recompile")
			return;
	}
	aBool = savedBools.top();
	savedBools.pop();
}

void Storage::Load(std::string& aString)
{
	//if you save with a certain serialization and then load with a changed one the ones you have changed will not be saved
	//so we are checking if the container is empty
	if (savedStrings.empty())
	{
		aString = "";
		LOG_WARN(Logger::Get("core"), "Couldn't load any more strings, because it wasn't included in the Serialize function at the last Script recompile")
			return;
	}
	aString = savedStrings.top();
	savedStrings.pop();
}

void Storage::Load(PhysicsCompHandle& aPhysicsCompHandle)
{
	//if you save with a certain serialization and then load with a changed one the ones you have changed will not be saved
	//so we are checking if the container is empty
	if (savedPhysicsHandles.empty())
	{
		aPhysicsCompHandle = PhysicsCompHandle();
		aPhysicsCompHandle.index = { 1231234 };
		LOG_WARN(Logger::Get("core"), "Couldn't load any more AudioCompHandle, because it wasn't included in the Serialize function at the last Script recompile")
			return;
	}

	aPhysicsCompHandle = savedPhysicsHandles.top();
	savedPhysicsHandles.pop();
}
