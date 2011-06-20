#include "arengine/ARScene.h"
#include "arengine/Config.h"
#include "arengine/Singleton.h"
#include "arengine/SmartSingleton.h"
#include "arengine/KeyboardHandler.h"
#include "arengine/Logger.h"

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
using namespace arengine;
int main()
{
	osgViewer::Viewer viewer;
	// Initialize logger
	Logger* logger = Singleton<Logger>::getInstance();
	logger->init("log.txt", 4);

	// Read config file
	Config *config = Singleton<Config>::getInstance();
	config->readConfig("mastercv.conf");
	ref_ptr<ARScene> arscene = SmartSingleton<ARScene>::getInstance();
	arscene->start();
	viewer.setSceneData(arscene->getSceneData());
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(SmartSingleton<KeyboardHandler>::getInstance());
	viewer.run();
}