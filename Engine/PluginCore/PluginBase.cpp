#include "PluginBase.h"

#if !defined(WIN32) || !defined(_WIN32)
	#define __noop ((void)0)
#endif

PluginBase::PluginBase()
{
}


PluginBase::~PluginBase()
{
}

void PluginBase::SetConfigDirectories(std::shared_ptr<ConfigDirectories> aDirectories)
{
	__noop(aDirectories);
}
