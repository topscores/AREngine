#include "arengine/ARRoot.h"
#include "arengine/Util.h"

#include <osgUtil/Optimizer>

using namespace arengine;

ARRoot::ARRoot()
:m_activeSceneIdx(0)
,m_cam(NULL)
,m_vdoBackground(NULL)
{
}


ARRoot::~ARRoot()
{
}


void	
ARRoot::initCameraMatrix(osgART::Tracker *tracker)
{
	osg::ref_ptr<osgART::Calibration> calibration = tracker->getOrCreateCalibration();

	if (m_cam.valid())
	{
		ref_ptr<osg::Camera> newcam =  calibration->createCamera();
		replaceChild(m_cam, newcam);
		m_cam = newcam;
	}
	else
	{
		m_cam = calibration->createCamera();
		addChild(m_cam);
	}
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
		Util::log(__FUNCTION__, 2, "ARRoot::getScene() : Cannot find idx for %s", name.c_str());
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
		Util::log(__FUNCTION__, 2, "ARRoot::getIdxForSceneName() : Cannot find idx for %s", name.c_str());
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
					newActiveScene->initMarkerMatrixFromScene(curActiveScene);
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
		Util::log(__FUNCTION__, 2, "Cannot find idx for %s use idx = 0 instead", name.c_str());
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
		if (m_vdoBackground)
		{
			replaceChild(m_vdoBackground, background);
			// removeChild(m_vdoBackground);
		}
		else
		{
			addChild(background);
		}
		m_vdoBackground = background;
	}
	else
	{
		Util::log("ARRoot::setVideoBackground : NULL background", 3);
	}
}


int
ARRoot::getActiveSceneIdx()
{
	return m_activeSceneIdx;
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