#pragma once
#include <vector>
#include <string>
#include <functional>
#include <memory>

#include "Handle.h"
#include "ExportDetails.h"
#include "ForwardDecl.h"

//forward declarations
class Event;
class Level;
class Storage;

///SCRIPT_START declares the lvl variable, this variable is used to interface with other scripts and the engine.
#define SCRIPT_START(x)\
Level* x ## Interface::lvl = nullptr;

///SCRIPT_END creates the necessary functions to create a object and pass it to the engine
#define SCRIPT_END(x)\
extern "C" _declspec(dllexport) Object* Create##x()\
{\
	Object* tempObj = new x();\
	return tempObj;\
}\
extern "C" _declspec(dllexport) void Delete##x(Object* aObject)\
{\
	delete aObject;\
	aObject = nullptr;\
}\
extern "C" _declspec(dllexport) void SetLevel##x(Level* aLvl)\
{\
	x ## Interface::lvl = aLvl;\
}

#define INTERFACE_START(x)\
struct x ## Interface\
{\
	static Level* lvl;

#define INTERFACE_END()\
};

#define PUBLIC_FUNCTION(name, className)\
template<class... Arg> static auto name(ObjectHandle aHandle, Arg&&... args)\
{\
	return dynamic_cast<className*>(lvl->GetObjectPointer(aHandle))->name(std::forward<Arg>(args)...);\
}

//return lvl->CallFunction<x>(&x::name, aHandle, std::forward<Arg>(args)...);

/**
 * \brief The base class of all of the Scripts.
 * 
 * Declares the Start, Update and Delete functions that are used in the Script class
 * 
 * Has pointers to Parent and Children.
 * 
 * Has ways to register variables. These variables can be changed in the engine without recompilation.
 * 
 * Has Eventhandler which is able to create and listen to events.
 * 
 * \see Script()
 */
class Object
{
//functions
public: 
	CORE_API Object() = default;;
	CORE_API Object(const char* aName);
	CORE_API virtual ~Object() {};

	CORE_API virtual void Start() = 0;
	CORE_API virtual void Restart() = 0;
	CORE_API virtual void FixedUpdate() = 0;
	CORE_API virtual void Update(float aDeltatime) = 0;
	CORE_API virtual void Delete() = 0;
	
	CORE_API const char* GetName() const;
	CORE_API void SetName(const char* aName);
	
	/////////////////////
	//serialization
	/////////////////////
	/**
	 * \brief Is called to serialize and deserialize the script
	 * \param storage The Storage class that should be called
	 */
	CORE_API virtual void Serialize(Storage& storage) = 0;

protected:

private:

	CORE_API void SetLevel(Level* aLvl);;
	CORE_API void SetLogger(LoggerHandle aLogger);;

//variables
public:

	

protected:

	Level* lvl;
	LoggerHandle console;
	std::string name;

private:
	friend class Level;
	friend class ScriptCompiler;
	friend class ScriptLoader;
	friend class Script;

	std::shared_ptr<Script> script;
};