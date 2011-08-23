#include "arengine/AREngine.h"
#include "arengine/Singleton.h"
#include "arengine/SmartSingleton.h"

using namespace arengine;

Logger*
AREngine::getLogger()
{
	return Singleton<Logger>::getInstance();
}


Config*
AREngine::getConfig()
{
	return Singleton<Config>::getInstance();
}


ARScene*
AREngine::getARScene()
{
	return SmartSingleton<ARScene>::getInstance();
}

