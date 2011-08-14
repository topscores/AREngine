#include "arengine/ARScene.h"
#include "arengine/Util.h"
#include "arengine/Config.h"
#include "arengine/SmartSingleton.h"
#include "arengine/Marker.h"
#include "arengine/ObjPool.h"

#include <osgDB/Readfile>

#include <osgAudio/SoundManager.h>
#include <osg/DeleteHandler>

#include <osgART/GeometryUtils>
#include <osgART/PluginManager>
#include <osgART/VideoGeode>
#include <osgART/VideoLayer>
#include <osgART/MarkerCallback>

using namespace arengine;


ARScene::ARScene()
{
	m_rootNode = new ARRoot();
}


ARScene::~ARScene()
{
}


void
ARScene::init()
{
	// Create global components that will be used for all scenes
	initVideo();

	ref_ptr<osg::Node> videoBackground = createVideoBackground();
	m_tracker = createTracker();

	// Set root node with global components
	m_rootNode->initCameraMatrix(m_tracker.get());
	m_rootNode->setVideoBackground(videoBackground);
	
	// Initialize sound system
	osgAudio::SoundManager::instance()->init( 16, true );
	osgAudio::SoundManager::instance()->getEnvironment()->setDistanceModel(osgAudio::InverseDistance);
	osgAudio::SoundManager::instance()->getEnvironment()->setDopplerFactor(1);
}


void 
ARScene::start()
{
	Config* config = Singleton<Config>::getInstance();
	string sceneName = config->getActiveSceneName();
	m_rootNode->setActiveScene(sceneName);

	m_video->start();
}


ref_ptr<osg::Node>
ARScene::getSceneData()
{
	//return m_root.get();
	return m_rootNode.get();
}


ref_ptr<osgART::Tracker>
ARScene::getTracker()
{
	return m_tracker;
}


void
ARScene::destroy()
{
	// Very important to call before end of main!
	if (osg::Referenced::getDeleteHandler()) {
		osg::Referenced::getDeleteHandler()->setNumFramesToRetainObjects(0);
		osg::Referenced::getDeleteHandler()->flushAll();
	}
	osgAudio::SoundManager::instance()->shutdown();
}


void
ARScene::initVideo()
{
	// Check wheter to use web camera or video file as a video source
	Config *config = Singleton<Config>::getInstance();
	if (config->getAVIFileName().empty())
	{
		int video_id = osgART::PluginManager::instance()->load("osgart_video_artoolkit2");
		
		// Load video plugin
		m_video = dynamic_cast<osgART::Video *>(osgART::PluginManager::instance()->get(video_id));
		if (!m_video.valid())
		{
			Util::log("ARScene::CreateBackgroundVideo : Could not initialize video", 1);
		}

		// Flip or not flip images from video before using it
		osgART::VideoConfiguration *videoConfig = m_video.get()->getVideoConfiguration();

#ifdef WIN32
		if (!config->flipEnable())
		{
			videoConfig->deviceconfig = "Data\\WDM_camera_normal.xml";
		}
		else
		{
			videoConfig->deviceconfig = "Data\\WDM_camera_mirror.xml";
		}
#endif
		
#ifdef __APPLE__
		if (!config->flipEnable())
		{
			videoConfig->deviceconfig = "-nodialog";
		}
		else
		{
			videoConfig->deviceconfig = "-nodialog -fliph";
		}
#endif
		
	}
	else
	{
		// Code to load video from file go here
		Util::log("Using VDO as a background texture is currently unsupported", 1);
	}
}


osg::ref_ptr<osg::Node>
ARScene::createVideoBackground()
{
	// check if loading the plugin was successful
	if (!m_video.valid()) 
	{
		initVideo();
	}

	// Open the video. This will not yet start the video stream but will
	// get information about the format of the video which is essential
	// for the connected tracker
	m_video->open();

	ref_ptr<osgART::VideoLayer> videoLayer = new osgART::VideoLayer();
	videoLayer->setSize(*m_video.get());

	Config *config = Singleton<Config>::getInstance();
	ref_ptr<osgART::VideoGeode> videoGeode;
	if (config->useTexture2D())
	{
		videoGeode = new osgART::VideoGeode(osgART::VideoGeode::USE_TEXTURE_2D, m_video.get());
	}
	else
	{
		videoGeode = new osgART::VideoGeode(osgART::VideoGeode::USE_TEXTURE_RECTANGLE, m_video.get());
	}
	osgART::addTexturedQuad(*videoGeode, m_video->s(), m_video->t());
	videoLayer->addChild(videoGeode.get());
	videoLayer->getOrCreateStateSet()->setRenderBinDetails(0, "RenderBin");

	return videoLayer;
}


// m_rootNode must be valid before calling this method
ref_ptr<osgART::Tracker>
ARScene::createTracker()
{
	if (!m_video.valid())
	{
		initVideo();
	}

	int tracker_id	=	osgART::PluginManager::instance()->load("osgart_tracker_artoolkit2");
	ref_ptr<osgART::Tracker> tracker =	dynamic_cast<osgART::Tracker *>(osgART::PluginManager::instance()->get(tracker_id));

	osg::ref_ptr<osgART::Marker> marker;
	if (tracker.valid()) 
	{
		// access a field within the tracker
		osg::ref_ptr< osgART::TypedField<int> > _threshold = 
			reinterpret_cast< osgART::TypedField<int>* >(tracker->get("threshold"));

		// values can only be accessed through a get()/set() mechanism
		if (_threshold.valid()) 
		{			
			// set the threshold
			_threshold->set(100);

			/* check what we actually get */
			//osg::notify() << "Field 'threshold' = " << _threshold->get() << std::endl;
			char msg[100];
			sprintf(msg, "Field 'threshold' = %d", _threshold->get());
			Util::log(msg, 3);

		} 
		else 
		{
			//osg::notify() << "Field 'threshold' supported for this tracker" << std::endl;
			Util::log("Field 'threshold' supported for this tracker", 3);
		}		

		// get the tracker calibration object
		osg::ref_ptr<osgART::Calibration> calibration = tracker->getOrCreateCalibration();

		// load a calibration file
		if (!calibration->load(std::string("data/camera_para.dat"))) 
		{

			// the calibration file was non-existing or couldnt be loaded
			Util::log("Non existing or incompatible calibration file", 1);
		}

		// set the image source for the tracker
		tracker->setImage(m_video.get());
		osgART::TrackerCallback::addOrSet(m_rootNode.get(), tracker.get());
	} 
	else 
	{
		Util::log("Cannot initialize tracker", 1);
	}	

	return tracker;
}