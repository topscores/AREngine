#ifndef __ARSCENE_H__
#define __ARSCENE_H__

#include "arengine/Export"
#include "arengine/ARRoot.h"

#include <osg/Node>
#include <osg/Group>
#include <osgART/Tracker>
#include <osgART/Video>

using namespace osg;

namespace arengine
{
	
	class ARENGINE_EXPORT ARScene : public osg::Referenced
	{
	public:
		ARScene();
		~ARScene();
		
		// Must be called to initialize tracker and video
		void init();
		void start();
		ref_ptr<osg::Node> getSceneData();
		ref_ptr<osgART::Tracker> getTracker();

	private:
		void initVideo();
		ref_ptr<osg::Node> createVideoBackground();

		ref_ptr<osgART::Tracker> createTracker();

	private:
		int m_activeScene;
		ref_ptr<ARRoot>				m_rootNode;
		ref_ptr<osgART::Tracker>	m_tracker;
		ref_ptr<osgART::Video>		m_video;
	};

}

#endif