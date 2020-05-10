#include "PluginBase.h"
#include "NoopFunctions.h"

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
