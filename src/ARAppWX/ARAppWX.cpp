#include "wx/wx.h"

#include "arengine/Util.h"
#include "arengine/Logger.h"
#include "arengine/ARScene.h"
#include "arengine/Singleton.h"
#include "arengine/SmartSingleton.h"
#include "arengine/ObjPool.h"
#include "arengine/SceneObj.h"
#include "arengine/Config.h"
#include "arengine/Model.h"
#include "arenginewx/OSGFrame.h"
#include "arenginewx/LGSplashWx.h"


#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>

#include <string>
#include <iostream>

using namespace arengine;
using namespace arenginewx;
using namespace std;

class ARAppWX : public wxApp
{
public:
	bool OnInit();
};


// `Main program' equivalent, creating windows and returning main app frame
bool ARAppWX::OnInit()
{
	// Create the main frame window
	OSGFrame *frame = new OSGFrame(NULL, wxT("wxWidgets OSG Sample"),
		wxDefaultPosition, wxSize(800, 600));

	LGSplashWx *splash = new LGSplashWx(wxT(".\\huds\\splash.png"));
	splash->show();

	// wxString fname(argv[1]);
	// Initialize logger
	Logger* logger = Singleton<Logger>::getInstance();
	logger->init("log.txt", 4);

	// Read config file
	Config *config = Singleton<Config>::getInstance();
	config->readConfig("mastercv.conf");

	splash->SetMainFrame(frame, config->fullScreen());
	
	osgViewer::Viewer *viewer = frame->getViewer();

	//osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("cow.osg");
	//if (!loadedModel)
	//{
	//    std::cout << argv[0] <<": No data loaded." << std::endl;
	//    return false;
	//}
	//viewer->setSceneData(loadedModel.get());

	//SceneObjPool *pool = Singleton<SceneObjPool>::getInstance();
	//ref_ptr<SceneObj> obj = pool->getSceneObj("orbit").get();
	//viewer->setSceneData(obj);

	// load the scene.
	ref_ptr<ARScene> arscene = SmartSingleton<ARScene>::getInstance();
	viewer->setSceneData(arscene->getSceneData().get());
	arscene->start();

	splash->close();
	
	viewer->setCameraManipulator(new osgGA::TrackballManipulator);

	return true;
}

IMPLEMENT_APP(ARAppWX)
