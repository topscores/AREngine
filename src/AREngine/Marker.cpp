#include "arengine/Marker.h"
#include "arengine/ObjPool.h"
#include "arengine/SceneObj.h"
#include "arengine/Exception.h"
#include "arengine/Util.h"
#include "arengine/Singleton.h"
#include "arengine/SmartSingleton.h"
#include "arengine/Config.h"
#include "arengine/ARScene.h"

#include <osgART/Marker>
#include <osgART/MarkerCallback>
#include <osgART/TransformFilterCallback>

#include <sstream>

using namespace std;
using namespace arengine;


Marker::Marker(DataNode *markerNode)
{
	try
	{
		if (markerNode->getNodeName().compare("Marker") != 0)
		{
			throw Exception("Marker::Marker() : Not a Marker tag", 3);
		}

		m_markerName = markerNode->getAttributeAsString("name");

		m_initString = markerNode->getAttributeAsString("initString");
		if (m_initString.empty())
		{
			throw Exception("Marker::Marker() : InitString is empty", 2);
		}
		else
		{
			initMarkerMatrixCallback();
		}

		int countAssociatedModel = markerNode->countChild("Model");
		for (int i = 0;i < countAssociatedModel;i++)
		{
			DataNode *modelNode = markerNode->getChild("Model", i);
			string modelName = modelNode->getAttributeAsString("name");
			if (!modelName.empty())
			{
				addAssociatedObj(modelName);
			}
			else
			{
				throw Exception("Marker::Marker() : Cannot associate model which does not have a name", 2);
			}
		}
		getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");
	}
	catch (Exception err)
	{
		Util::log(err);
	}
}


Marker::~Marker()
{
}


int
Marker::getIdxForObjName(string name)
{
	if (name.empty())
	{
		// Util::log("Marker::getIdxForObjName() : Cannot find idx for empty name", 2);
		return -1;
	}
	else
	{
		int size = m_associatedObjs.size();
		for (int i = 0;i < size;i++)
		{
			if (name.compare(m_associatedObjs[i]->getObjName()) == 0)
			{
				return i;
			}
		}
		// stringstream sstr;
		// sstr << "Marker::getIdxForObjName() : Cannot find idx for " << name;
		// Util::log(sstr.str().c_str(), 2);
		return -1;
	}
}


ref_ptr<osgART::Marker>
Marker::getOsgMarker()
{
	return m_osgMarker;
}


string
Marker::getMarkerName()
{
	return m_markerName;
}


void
Marker::setActive(bool active)
{
	if (m_osgMarker.valid())
	{
		m_osgMarker->setActive(active);
	}
	else
	{
		Util::log("Marker::setActive() : Invalid pointer to marker", 2);
	}
}


void
Marker::initMarkerMatrixCallback()
{
	osgART::Tracker *tracker = SmartSingleton<ARScene>::getInstance()->getTracker();
	m_osgMarker = tracker->addMarker(m_initString);

	if (m_osgMarker.valid())
	{
		m_osgMarker->setActive(true);
		osgART::attachDefaultEventCallbacks(this, m_osgMarker.get());

		Config *config = Singleton<Config>::getInstance();
		// Reduce Jitter or not
		if (config->smoothMotion())
		{
			osgART::TransformFilterCallback* tfc = new osgART::TransformFilterCallback();
			osgART::addEventCallback(this, tfc);
			tfc->enableTranslationalSmoothing(true);
			tfc->enableRotationalSmoothing(true);
		}

	}
	else
	{
		stringstream sstr;
		sstr << "Marker::initMarkerMatrixCallback() : Cannot add marker for string = " << m_initString;
		Util::log(sstr.str().c_str(), 2);
	}
}


int
Marker::addAssociatedObj(string objName)
{
	// Check whether the model was already added
	vector<string>::iterator itr;
	for (itr = m_associatedObjNames.begin();itr != m_associatedObjNames.end();itr++)
	{
		string name = *itr;
		if (name.compare(objName) == 0)
		{
			return -1;
		}
	}

	// If not add it
	SceneObjPool* pool = Singleton<SceneObjPool>::getInstance();
	ref_ptr<SceneObj> obj = pool->getByName(objName);

	stringstream sstr;
	sstr << "Marker::addAssociatedObj() : find object for " << objName;
	Util::log(sstr.str().c_str(), 4);

	if (obj.valid())
	{
		m_associatedObjNames.push_back(objName);
		m_associatedObjs.push_back(obj);
		addChild(obj.get());
		Util::log("Marker::addAssociateObj() : successfully add obj", 4);
		return m_associatedObjs.size() - 1;
	}
	else
	{
		stringstream sstr;
		sstr << "Marker::addAssociatedObj() : Cannot find object for " << objName;
		Util::log(sstr.str().c_str(), 2);
		return -1;
	}
}


void
Marker::removeAssociatedObj(string objName)
{
	int removeIndex = -1;
	ref_ptr<SceneObj> removeObj = NULL;

	// Find removeIndex
	int i = 0;
	vector<string>::iterator itr;
	for (itr = m_associatedObjNames.begin();itr != m_associatedObjNames.end();itr++, i++)
	{
		string name = *itr;
		if (name.compare(objName) == 0)
		{
			removeIndex = i;
		}
	}

	// If the removing model is in list
	if (removeIndex != -1)
	{
		string name;
		removeObj = getAssociatedObj(removeIndex);
		name = removeObj->getObjName();

		m_associatedObjNames.erase(m_associatedObjNames.begin()+removeIndex, m_associatedObjNames.begin()+removeIndex+1);
		
		// Remove the node from marker node
		removeChild(removeObj.get());
		m_associatedObjs.erase(m_associatedObjs.begin()+removeIndex, m_associatedObjs.begin()+removeIndex+1);

		stringstream sstr;
		sstr << "Remove Object \""<< name <<"\" from marker" << m_markerName;
		Util::log(__FUNCTION__, sstr.str().c_str(), 3);
	}
}


void
Marker::removeAssociatedObj()
{
	ref_ptr<SceneObj> removeObj;
	string name;
	int n = countAssociatedObj();
	for (int i = 0;i < n;i++)
	{
		removeObj = m_associatedObjs[i];
		name = m_associatedObjNames[i];
		removeChild(removeObj.get());

		stringstream sstr;
		sstr << "Remove Object \""<< name <<"\" from marker" << m_markerName;
		Util::log(__FUNCTION__, sstr.str().c_str(), 3);
	}
	m_associatedObjs.clear();
	m_associatedObjNames.clear();
}


int
Marker::countAssociatedObj()
{
	return m_associatedObjs.size();
}


string
Marker::getAssociatedObjName(int index)
{
	if (index >= 0 && index < countAssociatedObj())
	{
		return m_associatedObjNames[index];
	}
	return string();
}


ref_ptr<SceneObj>
Marker::getAssociatedObj(int idx)
{
	if (idx >= 0 && idx < countAssociatedObj())
	{
		return m_associatedObjs[idx];
	}
	else
	{
		Util::log("Marker::getAssociatedObj() : Invalid object idx", 2);
		return NULL;
	}
}


ref_ptr<SceneObj>
Marker::getAssociatedObj(string name)
{
	int idx = getIdxForObjName(name);
	if (idx == -1)
	{
		stringstream sstr;
		sstr << "Marker::getAssociatedObj() : Cannot find idx for " << name;
		Util::log(sstr.str().c_str(), 2);
		return NULL;
	}
	else
	{
		getAssociatedObj(idx);
	}
}