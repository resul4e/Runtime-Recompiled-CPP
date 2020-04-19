#include "PluginBase.h"



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
