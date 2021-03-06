#include "arengine/ARScene.h"
#include "arengine/Util.h"
#include "arengine/Config.h"
#include "arengine/SmartSingleton.h"
#include "arengine/Marker.h"
#include "arengine/ObjPool.h"
#include "arengine/SDLSoundManager.h"
#include "arengine/CaptureDeviceManager.h"

#include <osgDB/Readfile>

#include <osgART/GeometryUtils>
#include <osgART/PluginManager>
#include <osgART/VideoGeode>
#include <osgART/VideoLayer>
#include <osgART/MarkerCallback>

#ifdef _WIN32
#	include <windows.h>
#	include <atlbase.h>
#	include <atlconv.h>
#	include <objbase.h>
#endif

using namespace arengine;

ARScene::ARScene()
:m_tracker(NULL)
,m_video(NULL)
{
	m_rootNode = new ARRoot();
}


ARScene::~ARScene()
{
}


void
ARScene::init()
{
	//// Init sound system
	SDLSoundManager *soundMgr = Singleton<SDLSoundManager>::getInstance();
	soundMgr->initAudio();

	CoInitialize(NULL);
	// Create global components that will be used for all scenes
	m_video = initVideo();

	ref_ptr<osg::Node> videoBackground = createVideoBackground();
	m_tracker = createTracker();

	// Set root node with global components
	m_rootNode->initCameraMatrix(m_tracker.get());
	m_rootNode->setVideoBackground(videoBackground);
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
	return m_rootNode.get();
}


ref_ptr<osgART::Tracker>
ARScene::getTracker()
{
	return m_tracker;
}


void
ARScene::release()
{
#ifdef WIN32
	ref_ptr<osg::Image> frame = m_video.get();
	CaptureDeviceManager::saveCapDevInfo(m_capdev, frame->s(), frame->t());
#endif
	m_video->stop();
	CoUninitialize();

	// Init sound system
	SDLSoundManager *soundMgr = Singleton<SDLSoundManager>::getInstance();
	soundMgr->closeAudio();
	
	// Need to do this to force ref_ptr to unref, otherwise will cause core dump on osx
	// This might have something to do with static variable memory releasing in osx
	m_rootNode = NULL;
	m_video = NULL;
	m_tracker = NULL;
}


void
ARScene::setVideoConfig(ref_ptr<osgART::Video> video, 
						bool showDialog,
						int frame_width,
						int frame_height)
{
	if (video.valid())
	{
		Config *config = Singleton<Config>::getInstance();
		
		// Flip or not flip images from video before using it
		osgART::VideoConfiguration *videoConfig = video.get()->getVideoConfiguration();

		string conf = CaptureDeviceManager::createConfigString(showDialog,
												config->flipEnable(),
												frame_width,
												frame_height);

		videoConfig->deviceconfig = conf.c_str();
	}
}


ref_ptr<osgART::Video>
ARScene::initVideo()
{
	ref_ptr<osgART::Video> video;
	// Check wheter to use web camera or video file as a video source
	Config *config = Singleton<Config>::getInstance();
	if (config->getAVIFileName().empty())
	{
		int video_id = osgART::PluginManager::instance()->load("osgart_video_artoolkit2");
		
		// Load video plugin
		video = dynamic_cast<osgART::Video *>(osgART::PluginManager::instance()->get(video_id));
		if (!video.valid())
		{
			Util::log("ARScene::CreateBackgroundVideo : Could not initialize video", 1);
		}

#ifdef WIN32
		if (CaptureDeviceManager::getDeviceCount() < 1)
		{
			MessageBox(NULL, TEXT("Fatal Error"), TEXT("Capture Device not found, program will now exit"), MB_OK);
			Util::log(__FUNCTION__, "Capture device not found", 1);
		}

		// Capture device name has not yet been set before
		// Choose the first one in list
		int frame_width, frame_height;
		CaptureDeviceManager::loadCapDevInfo(m_capdev, frame_width, frame_height);
		if (m_capdev == "")
		{
			Util::log(__FUNCTION__, 3, "No capture device from previous session, choose the first available one(%s)", m_capdev.c_str());
			m_capdev = CaptureDeviceManager::getDisplayName(0);
			setVideoConfig(video, false, 640, 480);
			video->open(CaptureDeviceManager::getAsSourceFilter(0));
		}
		else
		{
			CComPtr<IBaseFilter> pSrcFilter = CaptureDeviceManager::getAsSourceFilter(m_capdev);
			if (pSrcFilter)
			{
				Util::log(__FUNCTION__, 3, "Capture Device = %s", m_capdev.c_str());
				setVideoConfig(video, false, frame_width, frame_height);
				video->open(pSrcFilter);
			}
			else
			{
				Util::log(__FUNCTION__, 3, "Previous capture device not found, choose the first available one(%s)", m_capdev.c_str());
				m_capdev = CaptureDeviceManager::getDisplayName(0);
				setVideoConfig(video, false, 640, 480);
				video->open(CaptureDeviceManager::getAsSourceFilter(0));
			}
		}
#endif

#ifdef __APPLE__
		setVideoConfig(video, false);
		video->open();
#endif
		return video;
	}
	else
	{
		// Code to load video from file go here
		Util::log("Using VDO as a background texture is currently unsupported", 1);
		return NULL;
	}
}


osg::ref_ptr<osg::Node>
ARScene::createVideoBackground()
{
	// check if loading the plugin was successful
	if (!m_video.valid()) 
	{
		m_video = initVideo();
	}

	// Open the video. This will not yet start the video stream but will
	// get information about the format of the video which is essential
	// for the connected tracker
	//m_video->open();

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
		m_video = initVideo();
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
		if (!calibration->load(Util::getNativePath("data/camera_para.dat"))) 
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


#ifdef WIN32

void 
ARScene::changeCaptureDevice(int i)
{
	if (i >= 0 && i < CaptureDeviceManager::getDeviceCount())
	{
		m_video->close(false);
		m_video->open(CaptureDeviceManager::getAsSourceFilter(i));
		if (m_video.valid())
		{
			m_capdev = CaptureDeviceManager::getDisplayName(i);
			ref_ptr<osg::Node> videoBackground = createVideoBackground();
			if (m_tracker.valid())
			{
				m_rootNode->setVideoBackground(videoBackground);
				m_tracker->setImage(m_video.get());
				m_rootNode->initCameraMatrix(m_tracker);
				m_rootNode->setActiveScene(m_rootNode->getActiveSceneIdx());
				m_video->start();
			}
			else
			{
				Util::log(__FUNCTION__, 2, "Invalid tracker");
			}
		}
		else
		{
			Util::log(__FUNCTION__, 2, "Cannot create new capture device");
		}

	}
}


void 
ARScene::showPinProperties(HWND hWnd)
{
	m_video->close(false);

	setVideoConfig(m_video, true);
	m_video->open(CaptureDeviceManager::getAsSourceFilter(m_capdev));
	setVideoConfig(m_video, false);

	if (m_video.valid())
	{
		ref_ptr<osg::Node> videoBackground = createVideoBackground();
		if (m_tracker.valid())
		{
			m_rootNode->setVideoBackground(videoBackground);
			m_tracker->setImage(m_video.get());
			m_rootNode->initCameraMatrix(m_tracker);
			m_rootNode->setActiveScene(m_rootNode->getActiveSceneIdx());
			m_video->start();
		}
		else
		{
			Util::log(__FUNCTION__, 2, "Invalid tracker");
		}
	}
	else
	{
		Util::log(__FUNCTION__, 2, "Cannot create new capture device");
	}
}


void 
ARScene::showFilterProperties(HWND hWnd)
{
	if (m_video.valid())
	{
		m_video->showFilterProperties(hWnd);
	}
}

#endif

#ifdef __APPLE__

void
ARScene::showDeviceConfig()
{
	m_video->close(false);
	setVideoConfig(m_video, true);
	m_video->open();
	setVideoConfig(m_video, false);
	
	if (m_video.valid())
	{
		ref_ptr<osg::Node> videoBackground = createVideoBackground();
		if (m_tracker.valid())
		{
			m_rootNode->setVideoBackground(videoBackground);
			m_tracker->setImage(m_video.get());
			m_rootNode->initCameraMatrix(m_tracker);
			m_rootNode->setActiveScene(m_rootNode->getActiveSceneIdx());
			m_video->start();
		}
		else
		{
			Util::log(__FUNCTION__, 2, "Invalid tracker");
		}
	}
	else
	{
		Util::log(__FUNCTION__, 2, "Cannot create new capture device");
	}
}

#endif