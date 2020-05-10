#include "TransformPlugin.h"
#include "NoopFunctions.h"

TransformPlugin::TransformPlugin()
{
}


TransformPlugin::~TransformPlugin()
{
}

void TransformPlugin::Start()
{
}

void TransformPlugin::FixedUpdate()
{
}

void TransformPlugin::Update(float aDeltatime)
{
	__noop(aDeltatime);
}

void TransformPlugin::Delete()
{
}

PLUGIN_END(TransformPlugin)