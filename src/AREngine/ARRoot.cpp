#include "arengine/ARRoot.h"
#include "arengine/Util.h"

#include <osgUtil/Optimizer>
#include <osgAudio/SoundManager.h>

#include <sstream>

using namespace arengine;
using namespace std;


ARRoot::ARRoot()
{
	m_activeSceneIdx = 0;

	m_sound_root = new osgAudio::SoundRoot();
	this->addChild(m_sound_root);

}


ARRoot::~ARRoot()
{
}


void	
ARRoot::initCameraMatrix(osgART::Tracker *tracker)
{
	osg::ref_ptr<osgART::Calibration> calibration = tracker->getOrCreateCalibration();
	m_cam = calibration->createCamera();
	//m_cam->addChild(getActiveScene());
	addChild(m_cam);
}


ref_ptr<Scene>
ARRoot::getScene(int idx)
{
	if (idx >= 0 && idx < m_scene.size())
	{
		return m_scene[idx];
	}
	else
	{
		Util::log("ARRoot::getScene() : sceneNo out of bound", 3);
		return NULL;
	}
}


ref_ptr<Scene>
ARRoot::getScene(string name)
{
	int idx = getIdxForSceneName(name);
	if (idx == -1)
	{
		stringstream sstr;
		sstr << "ARRoot::getScene() : Cannot find idx for " << name;
		Util::log(sstr.str().c_str(), 2);
		return NULL;
	}
	else
	{
		getScene(idx);
	}
}


int
ARRoot::getIdxForSceneName(string name)
{
	// Default scene is number 0
	if (name.empty())
	{
		Util::log("ARRoot::getIdxForSceneName() : Cannot find idx for empty name", 2);
		return 0;
	}
	else
	{
		int size = m_scene.size();
		for (int i = 0;i < size;i++)
		{
			if (name.compare(m_scene[i]->getName()) == 0)
			{
				return i;
			}
		}
		stringstream sstr;
		sstr << "ARRoot::getIdxForSceneName() : Cannot find idx for " << name;
		Util::log(sstr.str().c_str(), 2);
		return 0;
	}
}


void
ARRoot::setActiveScene(int idx)
{
	if (m_scene.size() == 0)
	{
		Util::log("ARRoot::setActiveScene() : there is no scene registered", 3);
	}

	if (idx >= 0 && idx < m_scene.size())
	{
		if (m_cam.valid())
		{
			if (m_cam->getNumChildren() == 0)
			{
				m_cam->addChild(getScene(idx));
				m_activeSceneIdx = idx;
			}
			else
			{
				ref_ptr<Scene> curActiveScene = getActiveScene();
				ref_ptr<Scene> newActiveScene = getScene(idx);

				if (curActiveScene != newActiveScene)
				{
					curActiveScene->setActive(false);
					newActiveScene->setActive(true);
					m_cam->replaceChild(curActiveScene, newActiveScene);
					m_activeSceneIdx = idx;
				}
			}
			osgUtil::Optimizer optimizer;
			optimizer.optimize(this);
		}
		else
		{
			Util::log("ARRoot::setActiveScene() : Root node camera is missing", 1);
		}
	}
	else
	{
		Util::log("ARRoot::setActiveScene() : sceneNo out of bound", 3);
	}
}


void
ARRoot::setActiveScene(string name)
{
	int idx = getIdxForSceneName(name);
	if (idx == -1)
	{
		setActiveScene(0);
		stringstream sstr;
		sstr << "Cannot find idx for " << name << "use idx = 0 instead";
		Util::log(sstr.str().c_str(), 2);
	}
	else
	{
		setActiveScene(idx);
	}
}

int
ARRoot::addScene(Scene *scene)
{
	if (scene != NULL)
	{
		m_scene.push_back(scene);
		return getSceneCount() - 1;
	}
	else
	{
		Util::log("ARRoot::addScene() : Cannot add NULL scene", 2);
	}
}


int
ARRoot::getSceneCount()
{
	return m_scene.size();
}


void
ARRoot::setVideoBackground(osg::Node *background)
{
	if (background != NULL)
	{
		background->getOrCreateStateSet()->setRenderBinDetails(0, "RenderBin");
		addChild(background);
	}
	else
	{
		Util::log("ARRoot::setVideoBackground : NULL background", 3);
	}
}


ref_ptr<Scene>
ARRoot::getActiveScene()
{
	if (m_scene.size() == 0)
	{
		Util::log("ARRoot::getActiveScene() : there is no scene registered", 3);
		return NULL;
	}

	if (m_activeSceneIdx >= 0 && m_activeSceneIdx < m_scene.size())
	{
		return m_scene[m_activeSceneIdx];	
	}
	else
	{
		Util::log("ARRoot::getActiveScene() : activeSceneNo is invalid", 1);
	}
}