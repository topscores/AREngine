#ifndef __ARSCENE_H__
#define __ARSCENE_H__

#include "arengine/Export"
#include "arengine/SmartSingleton.h"
#include "arengine/ARRoot.h"

#include <osg/Node>
#include <osg/Group>
#include <osgART/Tracker>
#include <osgART/Video>

#ifdef _WIN32
#	include <windows.h>
#	include <dshow.h>
#endif
using namespace osg;

namespace arengine
{
	// This class should be access via SmartSingleton class
	class ARENGINE_EXPORT ARScene : public osg::Referenced
	{
	template<class ARScene> friend class SmartSingleton;
	public:
		~ARScene();
		
		// Must be called to initialize tracker and video
		void init();

		void start();

		ref_ptr<osg::Node> getSceneData();
		ref_ptr<osgART::Tracker> getTracker();
		
		// Important to call before the end of main
		void release();

#ifdef _WIN32
		void changeCaptureDevice(IBaseFilter *pSrcFilter);
		void showPinProperties(HWND hWnd);
		void showFilterProperties(HWND hWnd);
#endif
	
	private:
		ARScene();
		ARScene(const ARScene&);
		ARScene& operator=(const ARScene&);

	private:
		void setVideoConfig(ref_ptr<osgART::Video> video, bool showDialog);

		ref_ptr<osgART::Video> initVideo();
		ref_ptr<osg::Node> createVideoBackground();

		ref_ptr<osgART::Tracker> createTracker();

	public:
		int m_activeScene;
		ref_ptr<ARRoot>				m_rootNode;
		ref_ptr<osgART::Tracker>	m_tracker;
		ref_ptr<osgART::Video>		m_video;
	};

}

#endif