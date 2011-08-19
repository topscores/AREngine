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
	ARScene* arscene = SmartSingleton<ARScene>::getInstance();
	arscene->start();
	viewer.setSceneData(arscene->getSceneData());
	if (config->viewStat())
	{
		viewer.addEventHandler(new osgViewer::StatsHandler());
	}
	//viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.addEventHandler(new osgViewer::WindowSizeHandler());
	viewer.addEventHandler(SmartSingleton<KeyboardHandler>::getInstance());
	viewer.run();
	//arscene->destroy();
}

/*#include <osgDB/ReadFile>
#include <osg/Node>
#include <osgART/Foundation>
#include <osgART/PluginManager>

using namespace osg;

//template<class T>
//class Singleton
//{
//private:
//	static ref_ptr<T> sm_ptr;
//	Singleton();
//	Singleton(const Singleton&);
//	Singleton& operator=(const Singleton&);
		
//public:
//	static ref_ptr<T> & getInstance()
//	{
//		if(!sm_ptr.valid())
//		{
//			sm_ptr = new T();
//		}
//		return sm_ptr;
//	}
//};
	
//template<class T> 
//ref_ptr<T> Singleton<T>::sm_ptr;
	

class Test : public Referenced
{
public:
	Test(){}
	~Test(){}
	
	ref_ptr<osgART::Video> video;
	//ref_ptr<osg::Node> node;
};

//class Test2 : public Referenced
//{
//public:
//	Test2(){}
//	~Test2(){}
	
//	ref_ptr<osg::Node> node;
//};


int main()
{
	static ref_ptr<Test> test = new Test();//Singleton<Test>::getInstance();
	//static ref_ptr<Test2> test2 = new Test2();
	//test2->node = osgDB::readNodeFile("cow.osg");
	int _video_id = osgART::PluginManager::instance()->load("osgart_video_artoolkit2");
	test->video = dynamic_cast<osgART::Video*>(osgART::PluginManager::instance()->get(_video_id));
	//test->node = test2->node;
	return 0;
}*/
