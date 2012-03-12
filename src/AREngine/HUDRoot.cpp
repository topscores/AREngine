#include "arengine/HUDRoot.h"
#include "arengine/ObjPool.h"
#include "arengine/Util.h"

#define DEFAULT_HUD_WIDTH 800
#define DEFAULT_HUD_HEIGHT 600
using namespace arengine;

HUDRoot::HUDRoot()
{
	int hudHeight, hudWidth;
	hudWidth = DEFAULT_HUD_WIDTH;
	hudHeight = DEFAULT_HUD_HEIGHT;

	m_hudCamera = new osg::Camera();
	m_hudCamera->setViewMatrix(osg::Matrix::identity());
	m_hudCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	m_hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
	m_hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

	osg::StateSet *state = m_hudCamera->getOrCreateStateSet();
	state->setMode(GL_BLEND, osg::StateAttribute::ON);
	state->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	//state->setMode(GL_LIGHTING, GL_FALSE);
	state->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	resizeHUD(0, hudWidth, 0, hudHeight);
	addChild(m_hudCamera);
}


HUDRoot::HUDRoot(DataNode *hudNode)
{
	int hudHeight, hudWidth;
	hudWidth = hudNode->getAttributeAsInt("width");
	hudHeight = hudNode->getAttributeAsInt("height");
	// Default hud resolution = 800x600
	if (hudWidth == 0 && hudHeight == 0)
	{
		hudWidth = 800;
		hudHeight = 600;
	}

	m_hudCamera = new osg::Camera();
	m_hudCamera->setViewMatrix(osg::Matrix::identity());
	m_hudCamera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	m_hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
	m_hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);

	osg::StateSet *state = m_hudCamera->getOrCreateStateSet();
	state->setMode(GL_BLEND, osg::StateAttribute::ON);
	state->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	//state->setMode(GL_LIGHTING, GL_FALSE);
	state->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	resizeHUD(0, hudWidth, 0, hudHeight);
	addChild(m_hudCamera);

	int imageCount = hudNode->countChild("Image");
	for (int i = 0;i < imageCount;i++)
	{
		DataNode *imageNode = hudNode->getChild("Image", i);
		string name = imageNode->getAttributeAsString("name");
		addHUD(name);
	}
}


HUDRoot::~HUDRoot()
{
}


void
HUDRoot::addHUD(string name)
{
	if (!name.empty())
	{
		// Check whether the object is already in list or not
		vector< ref_ptr<SceneObj> >::iterator itr;
		for (itr = m_hudList.begin();itr != m_hudList.end();itr++)
		{
			SceneObj *obj = *itr;
			if (obj->getObjName().compare(name) == 0)
			{
				stringstream sstr;
				sstr << "Object \""<< name<<"\" is already in list";
				Util::log(__FUNCTION__, sstr.str().c_str(), 3);
				return;
			}
		}

		// Search for obj and add to list
		SceneObj *obj = Singleton<SceneObjPool>::getInstance()->getByName(name);
		if (obj != NULL)
		{
			m_hudList.push_back(obj);
			m_hudCamera->addChild(obj);

			stringstream sstr;
			sstr << "Add Object \""<< name<<"\" to list";
			Util::log(__FUNCTION__, sstr.str().c_str(), 3);
		}
		else
		{
			Util::log(__FUNCTION__, "Unknown HUD name", 2);
		}
	}
	else
	{
		Util::log(__FUNCTION__, "HUD's name cannot be empty", 2);
	}
}


void
HUDRoot::removeHUD(string name)
{
	if (name.empty())
		return;

	int removeIndex = -1;
	ref_ptr<SceneObj> removeObj = NULL;

	// Find removeIndex
	int i = 0;
	vector< ref_ptr<SceneObj> >::iterator itr;
	for (itr = m_hudList.begin();itr != m_hudList.end();itr++, i++)
	{
		string objName = (*itr)->getObjName();
		if (name.compare(objName) == 0)
		{
			removeIndex = i;
			stringstream sstr;
			sstr << "Object found at position " << i;
			Util::log(__FUNCTION__, sstr.str().c_str(), 3);
		}
	}

	// If the removing hud is in list
	if (removeIndex != -1)
	{
		// Remove the node from hud node
		m_hudCamera->removeChild(m_hudList.at(removeIndex));
		m_hudList.erase(m_hudList.begin()+removeIndex, m_hudList.begin()+removeIndex+1);
		stringstream sstr;
		sstr << "Remove Object \""<< name <<"\" from list";
		Util::log(__FUNCTION__, sstr.str().c_str(), 3);
	}
}


void
HUDRoot::removeHUD()
{
	ref_ptr<SceneObj> removeObj;
	int n = m_hudList.size();
	for (int i = 0;i < n;i++)
	{
		removeObj = m_hudList[i];
		string name = removeObj->getObjName();
		m_hudCamera->removeChild(removeObj);

		stringstream sstr;
		sstr << "Remove Object \""<< name <<"\" from list";
		Util::log(__FUNCTION__, sstr.str().c_str(), 3);
	}
	m_hudList.clear();
}


ref_ptr<osg::Node>
HUDRoot::getHUD(int id)
{
	if (m_hudList.size() == 0)
	{
		return NULL;
	}

	if (id >= 0 && id < m_hudList.size())
	{
		return m_hudList[id];
	}
	else
	{
		Util::log(__FUNCTION__, "Invalid id", 2);
	}
}


ref_ptr<osg::Node>
HUDRoot::getHUD(string name)
{
	if (!name.empty())
	{
		int size = m_hudList.size();
		for (int i = 0;i < size;i++)
		{
			if (name == m_hudList[i]->getName())
			{
				return m_hudList[i];
			}
		}
		return NULL;
	}
	else
	{
		Util::log(__FUNCTION__, "Name is empty", 2);
	}
	return NULL;
}


vector< ref_ptr<SceneObj> >
HUDRoot::getHUDList()
{
	return m_hudList;
}


void
HUDRoot::resizeHUD(int xMin, int xMax, int yMin, int yMax)
{
	if (!m_hudCamera)
	{
		Util::log(__FUNCTION__, "m_hudCamera has not been initialized", 2);
		return;
	}

	m_hudCamera->setProjectionMatrixAsOrtho2D(xMin, xMax, yMin, yMax);
}


void
HUDRoot::processImageData(DataNode *node)
{
}