#include "arengine/AREngine.h"
#include "arengine/Singleton.h"
#include "arengine/SmartSingleton.h"
#include "arengine/ObjPool.h"
#include "arengine/Action.h"
#include "arengine/SceneObj.h"
#include "arengine/Util.h"

using namespace arengine;

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