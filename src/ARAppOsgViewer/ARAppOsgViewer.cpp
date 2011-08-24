#include "arengine/AREngine.h"
#include "arengine/ARScene.h"
#include "arengine/Singleton.h"
#include "arengine/SmartSingleton.h"
#include "arengine/KeyboardHandler.h"
#include "arengine/Logger.h"
#include "arengine/Config.h"

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

using namespace arengine;

int main()
{
	AREngine::init("ARAppOsgViewer", "log.txt", 4, "mastercv.conf");
	osgViewer::Viewer viewer;
	ref_ptr<ARScene> arscene = AREngine::getARScene();
	arscene->start();
	viewer.setSceneData(arscene->getSceneData());
	Config *config = AREngine::getConfig();
	if (config->viewStat())
	{
		viewer.addEventHandler(new osgViewer::StatsHandler());
	}
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(SmartSingleton<KeyboardHandler>::getInstance());
	viewer.run();
	AREngine::release();
}