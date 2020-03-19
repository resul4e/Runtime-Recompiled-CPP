#include "stdafx.h"
#include "Object.h"

#include "Level.h"

const char* Object::GetName() const
{
	return name.c_str();
}

void Object::SetName(const char* aName)
{
	name = aName;
}

void Object::SetLevel(Level* aLvl)
{
	lvl = aLvl;
}

void Object::SetLogger(LoggerHandle aLogger)
{
	console = aLogger;
}
