#include "wx/wx.h"
#include "wx/msgdlg.h"

#include "arenginewx/OSGFrame.h"
#include "arenginewx/CameraCtrlFrame.h"

#include "arengine/AREngine.h"
#include "arengine/Util.h"
#include "arengine/Logger.h"
#include "arengine/ARScene.h"
#include "arengine/Singleton.h"
#include "arengine/SmartSingleton.h"
#include "arengine/ObjPool.h"
#include "arengine/SceneObj.h"
#include "arengine/Config.h"
#include "arengine/Model.h"
#include "arenginewx/LGSplashWx.h"

#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

#include <osgFX/Cartoon>

#include <string>
#include <iostream>

#include <boost/thread.hpp>

using namespace arengine;
using namespace arenginewx;
using namespace std;

class ARAppWX : public wxApp
{
public:
	bool OnInit();
	int OnExit();
};


// `Main program' equivalent, creating windows and returning main app frame
bool ARAppWX::OnInit()
{
	// Create the main frame window
	//OSGFrame *frame = new OSGFrame(NULL, wxT("wxWidgets OSG Sample"),
	//	wxDefaultPosition, wxSize(800, 600));

	CameraCtrlFrame *frame = new CameraCtrlFrame(NULL, wxT("wxWidgets OSG Sample"),
		wxDefaultPosition, wxSize(640, 480));

	if (!frame->WaitForCaptureDevice())
	{
		exit(0);
	}

	LGSplashWx *splash = new LGSplashWx(wxT("./huds/splash.gif"));
	splash->Show();

	boost::thread initThread(AREngine::init, "ARAppWX", "log.txt", 4, "mastercv.conf", false);
	//AREngine::init("ARAppWX", "log.txt", 4, "mastercv.conf");

	while (!AREngine::ready())
	{
		wxYield();
		Sleep(100);
	}
	initThread.join();

	if (!AREngine::isLoggerReady())
	{
		if (!frame->ContinueWithoutLog())
		{
			splash->Close();
			AREngine::release();
			exit(0);
		}
	}

	Config *config = AREngine::getConfig();
	
	splash->SetMainFrame(frame, config->fullScreen());
	
	osgViewer::Viewer *viewer = frame->getViewer();

	//osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("Models\\cow.osg");
	//if (!loadedModel)
	//{
	//    std::cout << argv[0] <<": No data loaded." << std::endl;
	//    return false;
	//}
	//viewer->setSceneData(loadedModel.get());

	//SceneObjPool *pool = Singleton<SceneObjPool>::getInstance();
	//ref_ptr<SceneObj> obj = pool->getByName("cow");
	//viewer->setSceneData(obj);

	// load the scene.
	ref_ptr<ARScene> arscene = AREngine::getARScene();
	viewer->setSceneData(arscene->getSceneData().get());

	//ref_ptr<osgFX::Cartoon> cartoon = new osgFX::Cartoon();
	//cartoon->addChild(arscene->getSceneData().get());
	//viewer->setSceneData(cartoon);

	//if (config->viewStat())
	//{
	//	viewer->addEventHandler(new osgViewer::StatsHandler());
	//}

	arscene->start();

	splash->Close();
	
	//viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	frame->Show(true);
	

	return true;
}


int
ARAppWX::OnExit()
{
	AREngine::release();
	return 0;
}


IMPLEMENT_APP(ARAppWX)
