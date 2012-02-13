#include "arengine/AREngine.h"
#include "arengine/Singleton.h"
#include "arengine/SmartSingleton.h"
#include "arengine/ObjPool.h"
#include "arengine/Action.h"
#include "arengine/SceneObj.h"
#include "arengine/Util.h"
#include "arengine/CaptureDeviceManager.h"

using namespace arengine;

bool
AREngine::isCaptureDeviceReady()
{
#ifdef WIN32
	if (CaptureDeviceManager::getDeviceCount() < 1)
	{
		return false;
	}
	else
	{
		return true;
	}
#else
	return true;
#endif
}

void
AREngine::init(string appName, 
			   string logFileName,
			   int	  logLevel,
			   string configFileName)
{
	// Initialize logger
	Logger* logger = AREngine::getLogger();
	logger->init(logFileName, logLevel, appName);
	
	// Read config file
	Config *config = AREngine::getConfig();
	config->readConfig(configFileName);
}


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


KeyboardHandler*
AREngine::getKeyboardHandler()
{
	return SmartSingleton<KeyboardHandler>::getInstance();
}


void
AREngine::release()
{
	ref_ptr<ARScene> arscene = SmartSingleton<ARScene>::getInstance();
	arscene->release();
	
	ActionPool *actionPool = Singleton<ActionPool>::getInstance();
	actionPool->release();
	
	SceneObjPool *sceneObjPool = Singleton<SceneObjPool>::getInstance();
	sceneObjPool->release();
}