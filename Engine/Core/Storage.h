#pragma once
#include <stack>
#include "ExportDetails.h"
#include "Transform/Vector.h"
#include "Logger.h"

/**
 * \brief Handles all of the serialization during recompilation
 * 
 * Has support for:
 *  - RenderCompHandle
 *  - float
 * 
 * Scripts should override the Object::Serialize function and call the Serialize function of this class
 */
class Storage
{
//functions
public:
	CORE_API Storage(): save(true){};
	CORE_API ~Storage(){};
	/**
	 * \brief Goes through each parameter and saves them in their respective containers or loads their values from the container.
	 * \tparam T The next parameter in the argument list.
	 * \tparam Args The argument list, all arguments except the first one are stored in this parameter.
	 * 
	 * \param t The next parameter in the argument list.
	 * \param aArgs The argument list, all arguments except the first one are stored in this parameter.
	 * \note Serialization function expects the values of each type to be supplied in same order as they are saved.
	 */
	template <typename T, typename ... Args>
	void Serialize(T& t, Args &... aArgs);

private:
	friend class ScriptCompiler;

	///empty serialize function used to break the template recursion
	CORE_API void Serialize() const {}

	//////////////////
	//SAVING
	//////////////////
	/**
	 * \brief saves the RenderCompHandle in a stack
	 * \param aRenderCompHandle		The RenderCompHandle to be saved.
	 */
	CORE_API void Save(const RenderCompHandle& aRenderCompHandle);
	/**
	* \brief saves the ObjectHandle in a stack
	* \param aObjectHandle		The ObjectHandle to be saved.
	*/
	CORE_API void Save(const ObjectHandle& aObjectHandle);
	/**
	* \brief Saves the AudioCompHandle in a stack.
	* \param aHandle		The AudioCompHandles to be saved
	*/
	CORE_API void Save(const AudioCompHandle& aAudioCompHandle);
	/**
	* \brief Saves the AudioCompHandle in a stack.
	* \param aHandle		The AudioCompHandles to be saved
	*/
	CORE_API void Save(const SpriteCompHandle& aSpriteCompHandle);
	/**
	* \brief Saves the AnimationCompHandle in a stack.
	* \param aHandle		The AnimationCompHandle to be saved
	*/
	CORE_API void Save(const AnimationCompHandle& aAnimationCompHandle);
	/**
	* \brief Saves the PhysicsCompHandle in a stack.
	* \param aHandle		The PhysicsCompHandle to be saved
	*/
	CORE_API void Save(const PhysicsCompHandle& aPhysicsCompHandle);
	/**
	* \brief Saves the float in a stack.
	* \param aFloat		The float to be saved
	*/
	CORE_API void Save(const float& aFloat);
	/**
	* \brief Saves the vec2f in a stack.
	* \param aVec2f		The vec2f to be saved
	*/
	CORE_API void Save(const glm::vec2& aVec2f);
	/**
	* \brief Saves the bool in a stack.
	* \param aBool		The bool to be saved
	*/
	CORE_API void Save(const bool& aBool);
	/**
	* \brief Saves the string in a stack.
	* \param aString		The string to be saved
	*/
	CORE_API void Save(const std::string& aString);
	/**
	* \brief Saved a vector of one of the supported objects.
	* \param aVector		The vector to be saved.
	*/
	template<typename ValueType, typename AllocType>
	void Save(const std::vector<ValueType, AllocType>& aVector);

	//////////////////
	//LOADING
	//////////////////

	/**
	 * \brief fills the supplied RenderCompHandle with the next RenderCompHandle in the stack
	 * \param aRenderCompHandle		The RenderCompHandle that is being filled.
	 */
	CORE_API void Load(RenderCompHandle& aRenderCompHandle);
	/**
	 * \brief Fills the supplied ObjectHandle with the next ObjectHandle in the stack
	 * \param aObjectHandle		The ObjectHandle that is being filled.
	 */
	CORE_API void Load(ObjectHandle& aObjectHandle);
	/**
	* \brief Fills the supplied SpriteCompHandle with the next SpriteCompHandle in the stack
	* \param aSpriteCompHandle		The SpriteCompHandle that is being filled.
	*/
	CORE_API void Load(SpriteCompHandle& aSpriteCompHandle);
	/**
	* \brief Fills the supplied AnimationCompHandle with the next SpriteCompHandle in the stack
	* \param aAnimationCompHandle		The AnimationCompHandle that is being filled.
	*/
	CORE_API void Load(AnimationCompHandle& aAnimationCompHandle);
	/**
	* \brief Fills the supplied PhysicsCompHandle with the next SpriteCompHandle in the stack
	* \param aAnimationCompHandle		The PhysicsCompHandle that is being filled.
	*/
	CORE_API void Load(PhysicsCompHandle& aPhysicsCompHandle);
	/**
	* \brief fills the supplied float with the next float in the stack
	* \param aFloat		The float that is being filled.
	*/
	CORE_API void Load(float& aFloat);
	/**
	* \brief fills the supplied vec2 with the next vec2 in the stack
	* \param aVec2f		The vec2f to be filled
	*/
	CORE_API void Load(glm::vec2& aVec2f);
	/**
	* \brief fills the supplied AudioCompHandle with the next AudioCompHandle in the stack
	* \param aHandle		The AudioCompHandle to be filled
	*/
	CORE_API void Load(AudioCompHandle& aHandle);
	/**
	* \brief fills the supplied bool with the next bool in the stack
	* \param aBool		The bool to be filled
	*/
	CORE_API void Load(bool& aBool);
	/**
	* \brief fills the supplied string with the next string in the stack
	* \param aString		The string to be filled
	*/
	CORE_API void Load(std::string& aString);
	/**
	* \brief Saves a vector of one of the supported objects
	* \param aVector		The vector to be filled
	*/
	template<typename ValueType, typename AllocType>
	void Load(std::vector<ValueType, AllocType>& aVector);

//variables
public:
private:
	///if the Serialization should load or save
	bool save;
	///All of the saved RenderCompHandles
	std::stack<RenderCompHandle> savedRenderHandles;
	///All of the saved ObjectHandles
	std::stack<ObjectHandle> savedObjectHandles;
	///All of the saved AudioHandles
	std::stack<AudioCompHandle> savedAudioHandles;
	///All of the saved SpriteCompHandle
	std::stack<SpriteCompHandle> savedSpriteHandles;
	///All of the saved AnimationCompHandle
	std::stack<AnimationCompHandle> savedAnimationHandles;
	///All of the saved PhysicsCompHandle
	std::stack<PhysicsCompHandle> savedPhysicsHandles;
	///All of the saved floats
	std::stack<float> savedFloats;
	///All of the saved vecs
	std::stack<glm::vec2> savedVec2s;
	///All of the saved bools
	std::stack<bool> savedBools;
	///the size of each of the vectors that are saved
	std::stack<std::string> savedStrings;
	///the size of each of the vectors that are saved
	std::stack<size_t> savedVectors;

};



template <typename T, typename ... Args>
void Storage::Serialize(T& aT, Args&... aArgs)
{
	if(save)
	{
		Serialize(aArgs...);
		Save(aT);
	}
	else
	{
		Load(aT);
		Serialize(aArgs...);
	}
}

template<typename ValueType, typename AllocType>
void Storage::Save(const std::vector<ValueType, AllocType>& aVector)
{
	savedVectors.push(aVector.size());
	for(int i =0; i < aVector.size(); i++)
	{
		Save(aVector.at(i));
	}
}

template<typename ValueType, typename AllocType>
void Storage::Load(std::vector<ValueType, AllocType>& aVector)
{
	if(savedVectors.empty())
	{
		LOG_ERROR(Logger::Get("core"), "There are no more vectors to deserialize, this is probably because you didn't add it when serializing.");
		return;
	}
	size_t vectorSize = savedVectors.top();
	savedVectors.pop();

	for(int i= vectorSize - 1; i > 0; i--)
	{
		ValueType tempValue;
		Load(tempValue);
		aVector.push_back(tempValue);
	}
}