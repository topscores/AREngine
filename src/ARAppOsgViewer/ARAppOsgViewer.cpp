#include "arengine/AREngine.h"
#include "arengine/ARScene.h"
#include "arengine/Singleton.h"
#include "arengine/SmartSingleton.h"
#include "arengine/KeyboardHandler.h"
#include "arengine/Logger.h"
#include "arengine/Config.h"
#include "arengine/ResetAnimationAction.h"

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

using namespace arengine;

int main()
{
	AREngine::init("ARAppOsgViewer", "log.txt", 4, "mastercv.conf");
	osgViewer::Viewer viewer;
	ref_ptr<ARScene> arscene = AREngine::getARScene();
	arscene->start();
	ref_ptr<osg::Node> root = arscene->getSceneData();
	root->accept(ResetSequenceVisitor());
	root->accept(ResetAnimationPathVisitor());
	viewer.setSceneData(root.get());
	Config *config = AREngine::getConfig();
	if (config->viewStat())
	{
		viewer.addEventHandler(new osgViewer::StatsHandler());
	}
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(AREngine::getKeyboardHandler());
	viewer.run();
	AREngine::release();
}