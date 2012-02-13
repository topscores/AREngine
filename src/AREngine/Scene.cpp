#include "arengine/ARScene.h"
#include "arengine/SmartSingleton.h"
#include "arengine/Scene.h"
#include "arengine/Util.h"
#include "arengine/Exception.h"
#include "arengine/ConditionalCallback.h"

#include <osgART/MarkerCallback>
#include <osgART/Tracker>

#include <sstream>

using namespace arengine;
using namespace std;

Scene::Scene(DataNode *sceneNode)
{
	try
	{
		if (sceneNode->getNodeName().compare("Scene") != 0)
		{
			throw Exception("Scene::Scene() : Not a Scene tag", 2);
		}

		if (!sceneNode->getAttributeAsString("name").empty())
		{
			m_name	= sceneNode->getAttributeAsString("name");
		}
		else
		{
			throw Exception("Scene::Scene() : Name field cannot be empty", 2);
		}

		// Process Marker tag data
		int markerCount = sceneNode->countChild("Marker");
		for (int i = 0;i < markerCount;i++)
		{
			DataNode *marker = sceneNode->getChild("Marker", i);
			processMarkerData(marker);
		}

		int hudCount = sceneNode->countChild("HUD");
		if (hudCount > 0)
		{
			DataNode *hudNode = sceneNode->getChild("HUD", 0);
			m_hudRoot = new HUDRoot(hudNode);
		}
		else
		{
			m_hudRoot = new HUDRoot();
		}
		addChild(m_hudRoot);

		int callbackCount = sceneNode->countChild("ConditionalCallback");
		for (int i = 0;i < callbackCount;i++)
		{
			DataNode *callbackNode = sceneNode->getChild("ConditionalCallback", i);
			ConditionalCallback *callback = new ConditionalCallback(callbackNode);
			addUpdateCallback(callback);

		}

		m_pendingActionCB = new PendingActionCallback();
		addUpdateCallback(m_pendingActionCB);

		getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");
	}
	catch (Exception err)
	{
		Util::log(err);
	}

}


Scene::~Scene()
{
}


int
Scene::getIdxForMarkerName(string name)
{
	if (name.empty())
	{
		Util::log("Scene::getIdxForMarkerName() : Cannot find idx for empty name", 2);
	}
	else
	{
		int size = m_markers.size();
		for (int i = 0;i < size;i++)
		{
			if (name.compare(m_markers[i]->getMarkerName()) == 0)
			{
				return i;
			}
		}
		// stringstream sstr;
		// sstr << "Scene::getIdxForMarkerName() : Cannot find idx for " << name;
		// Util::log(sstr.str().c_str(), 2);
		return -1;
	}
}


void
Scene::setActive(bool active)
{
	// Deactivate all marker
	ref_ptr<osgART::Tracker > tracker = SmartSingleton<ARScene>::getInstance()->getTracker();
	if (tracker.valid())
	{
		int markerCount = tracker->getMarkerCount();
		for (int i = 0;i < markerCount;i++)
		{
			ref_ptr<osgART::Marker> marker = tracker->getMarker(i);
			marker->setActive(false);
		}
	}
	else
	{
		Util::log("Scene::setActive() : track has not yet been initialized", 2);
		return;
	}

	// Activate this scene's markers just in case
	if (active)
	{
		int markerCount = m_markers.size();
		for (int i = 0;i < markerCount;i++)
		{
			m_markers[i]->setActive(true);
		}
	}
}


bool
Scene::getActive()
{
	return m_active;
}


ref_ptr<PendingActionCallback>
Scene::getPendingActionCallback()
{
	return m_pendingActionCB;
}


string
Scene::getName()
{
	return m_name;
}


int
Scene::addMarker(Marker *marker)
{
	if (marker != NULL)
	{
		m_markers.push_back(marker);
		addChild(marker);
		return m_markers.size() - 1;
	}
	else
	{
		Util::log("Scene::addMarker() : Cannot add NULL marker", 2);
		return -1;
	}
}


ref_ptr<Marker>
Scene::getMarker(int idx)
{
	if (m_markers.size() == 0)
	{
		Util::log("Scene::getMarker() : There is no markers registered", 2);
		return NULL;
	}

	if (idx >= 0 && idx < getMarkerCount())
	{
		return m_markers[idx];
	}
	else
	{
		Util::log("Scene::getMarker() : Invalid marker idx", 2);
		return NULL;
	}
}


ref_ptr<Marker>
Scene::getMarker(string name)
{
	int idx = getIdxForMarkerName(name);
	if (idx == -1)
	{
		// stringstream sstr;
		// sstr << "Scene::getMarker() : Cannot find idx for " << name;
		// Util::log(sstr.str().c_str(), 2);
		return NULL;
	}
	else
	{
		return getMarker(idx);
	}
}


ref_ptr<HUDRoot>
Scene::getHUDRoot()
{
	return m_hudRoot;
}


int
Scene::getMarkerCount()
{
	return m_markers.size();
}


void
Scene::processMarkerData(DataNode *markerNode)
{
	if (markerNode->getNodeName().compare("Marker") == 0)
	{
		Marker *marker = new Marker(markerNode);
		addMarker(marker);
	}
}