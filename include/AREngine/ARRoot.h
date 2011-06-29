#ifndef __ARROOT_H__
#define __ARROOT_H__

#include "arengine/Export"
#include "arengine/Scene.h"

#include <osg/Node>
#include <osg/Camera>
#include <osg/Group>
#include <osgART/Tracker>
#include <osgAudio/SoundRoot.h>

#include <vector>

using namespace osg;
using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT ARRoot : public osg::Group
	{
	public:
		ARRoot();
		~ARRoot();

		// Init projection matrix and model view matrix from calibration parameters
		void	initCameraMatrix(osgART::Tracker *tracker);

		ref_ptr<Scene> getScene(int idx);
		ref_ptr<Scene> getScene(string name);

		int		getIdxForSceneName(string name);
		
		// Return scene's idx
		int		addScene(Scene *scene);
		int		getSceneCount();

		void	setActiveScene(int idx);
		void	setActiveScene(string name);
		void	setVideoBackground(osg::Node *background);


	private:
		ref_ptr<Scene>	getActiveScene();

	public:
		ref_ptr<osg::Camera>		m_cam;
		ref_ptr<osgAudio::SoundRoot> m_sound_root;
		vector<ref_ptr<Scene>>		m_scene;
		int							m_activeSceneIdx;
	};

}

#endif 