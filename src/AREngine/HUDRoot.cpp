#include "arengine/HUDRoot.h"
#include "arengine/ObjPool.h"
#include "arengine/Util.h"

#define DEFAULT_HUD_WIDTH 800
#define DEFAULT_HUD_HEIGHT 600
using namespace arengine;

//////////////////////////////////////////////////////////////////////////
// HUDLayer

HUDLayer::HUDLayer(int layerid)
{
	initLayer(DEFAULT_HUD_WIDTH, DEFAULT_HUD_HEIGHT, layerid);
}


HUDLayer::HUDLayer(DataNode *hudNode, int layerid)
{
	int width, height;
	width = hudNode->getAttributeAsInt("width");
	height = hudNode->getAttributeAsInt("height");
	// Default hud resolution = 800x600
	if (width == 0 && height == 0)
	{
		width = DEFAULT_HUD_WIDTH;
		height = DEFAULT_HUD_HEIGHT;
	}

	initLayer(width, height, layerid);

	int imageCount = hudNode->countChild("Image");
	for (int i = 0;i < imageCount;i++)
	{
		DataNode *imageNode = hudNode->getChild("Image", i);
		string name = imageNode->getAttributeAsString("name");
		addHUD(name);
	}
}


void
HUDLayer::addHUD(string name)
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
				Util::log(__FUNCTION__, 3, "Layer %d: Object %s is already in list", m_layerid, name.c_str());
				return;
			}
		}

		// Search for obj and add to list
		SceneObj *obj = Singleton<SceneObjPool>::getInstance()->getByName(name);
		if (obj != NULL)
		{
			m_hudList.push_back(obj);
			addChild(obj);

			Util::log(__FUNCTION__, 3, "Layer %d: Add object %s to list", m_layerid, name.c_str());
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
HUDLayer::removeHUD(string name)
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
			Util::log(__FUNCTION__, 3, "Layer %d: Object found at position %d", m_layerid, i);
		}
	}

	// If the removing hud is in list
	if (removeIndex != -1)
	{
		// Remove the node from hud node
		removeChild(m_hudList.at(removeIndex));
		m_hudList.erase(m_hudList.begin()+removeIndex, m_hudList.begin()+removeIndex+1);
		Util::log(__FUNCTION__, 3, "Layer %d: Remove object %s", m_layerid, name.c_str());
	}
}


void
HUDLayer::removeHUD()
{
	ref_ptr<SceneObj> removeObj;
	int n = m_hudList.size();
	for (int i = 0;i < n;i++)
	{
		removeObj = m_hudList[i];
		string name = removeObj->getObjName();
		removeChild(removeObj);

		Util::log(__FUNCTION__, 3, "Remove object %s", name.c_str());
	}
	m_hudList.clear();
}


ref_ptr<osg::Node>
HUDLayer::getHUD(int id)
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
		Util::log(__FUNCTION__, "Invalid id", 3);
		return NULL;
	}
}


ref_ptr<osg::Node>
HUDLayer::getHUD(string name)
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
HUDLayer::getHUDList()
{
	return m_hudList;
}


void
HUDLayer::initLayer(int width, int height, int layerid)
{
	m_width = width;
	m_height = height;

	setViewMatrix(osg::Matrix::identity());
	setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	setRenderOrder(osg::Camera::POST_RENDER);
	setClearMask(GL_DEPTH_BUFFER_BIT);

	osg::StateSet *state = getOrCreateStateSet();
	state->setMode(GL_BLEND, osg::StateAttribute::ON);
	state->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
	state->setRenderBinDetails(2 + layerid, "RenderBin");
	state->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	resize(0, m_width, 0, m_height);
	m_layerid = layerid;
}


void
HUDLayer::resize(int xMin, int xMax, 
				 int yMin, int yMax)
{
	if (xMin < xMax && yMin < yMax)
	{
		setProjectionMatrixAsOrtho2D(xMin, xMax, yMin, yMax);
	}
	else
	{
		Util::log(__FUNCTION__, 2, "Cannot initialize HUD layer %d with xMin=%d,xMax=%d,yMin=%d,yMax=%d",
			m_layerid, xMin, xMax, yMin, yMax);
	}
}

//////////////////////////////////////////////////////////////////////////
// HUDRoot

HUDRoot::HUDRoot()
{
}


HUDRoot::~HUDRoot()
{
}


void
HUDRoot::addLayer(DataNode *hudNode, int layerid)
{
	if (!hasLayer(layerid))
	{
		m_layerList[layerid] = new HUDLayer(hudNode, layerid);
		addChild(m_layerList[layerid]);
	}
}


bool
HUDRoot::hasLayer(int layerid)
{
	return m_layerList.find(layerid) != m_layerList.end();
}


ref_ptr<HUDLayer>
HUDRoot::getLayer(int layerid)
{
	if (hasLayer(layerid))
	{
		return m_layerList[layerid];
	}
	else
	{
		return NULL;
	}
}


map<int, ref_ptr<HUDLayer> >
HUDRoot::getLayerList()
{
	return m_layerList;
}


void	
HUDRoot::addHUD(string name, int layerid)
{
	ref_ptr<HUDLayer> hudLayer = getLayer(layerid);
	if (hudLayer.valid())
	{
		hudLayer->addHUD(name);
	}
	else
	{
		hudLayer = new HUDLayer(layerid);
		m_layerList[layerid] = hudLayer;
		Util::log(__FUNCTION__, "Layer %d not found, create new layer", layerid);
		
		hudLayer->addHUD(name);
	}
}


void
HUDRoot::removeHUD(int layerid)
{
	ref_ptr<HUDLayer> hudLayer = getLayer(layerid);
	if (hudLayer.valid())
	{
		hudLayer->removeHUD();
	}
}


void
HUDRoot::removeHUD(string name, int layerid)
{
	ref_ptr<HUDLayer> hudLayer = getLayer(layerid);
	if (hudLayer.valid())
	{
		hudLayer->removeHUD(name);
	}
}


ref_ptr<osg::Node>
HUDRoot::getHUD(string name)
{
	map<int, ref_ptr<HUDLayer> >::iterator itr;
	for (itr=m_layerList.begin();itr!=m_layerList.end();itr++)
	{
		ref_ptr<HUDLayer> layer = itr->second;
		ref_ptr<osg::Node> node = layer->getHUD(name);
		if (node.valid())
		{
			return node;
		}
	}
	return NULL;
}


ref_ptr<osg::Node>
HUDRoot::getHUD(string name, int layerid)
{
	if (m_layerList[layerid] != NULL)
	{
		return m_layerList[layerid]->getHUD(name);
	}
	else
	{
		return NULL;
	}
}


vector< ref_ptr<SceneObj> >
HUDRoot::getHUDList()
{
	vector< ref_ptr<SceneObj> > hudlist;
	map<int, ref_ptr<HUDLayer> >::iterator itr;
	
	for (itr = m_layerList.begin(); itr != m_layerList.end();itr++)
	{
		ref_ptr<HUDLayer> layer = itr->second;
		vector< ref_ptr<SceneObj> > tmpList = layer->getHUDList();
		int n = tmpList.size();
		hudlist.insert(hudlist.end(), tmpList.begin(), tmpList.end());
	}

	return hudlist;
}