#include "arengine/AppearChecker.h"
#include "arengine/Util.h"

#include <sstream>

using namespace std;
using namespace arengine;

AppearChecker::AppearChecker(DataNode *checkerNode)
:m_marker(NULL),
m_scene(NULL)
{
	if (checkerNode->getNodeName().compare("Appear") == 0)
	{
		m_markerName = checkerNode->getAttributeAsString("markerName");
		m_objName = checkerNode->getAttributeAsString("objName");
	}
	else
	{
		Util::log(__FUNCTION__, "Not an Appear tag", 2);
	}
}


AppearChecker::~AppearChecker()
{
}


bool
AppearChecker::conditionValid(osg::Node *node)
{
	if (!m_marker.valid() || !m_scene.valid())
	{
		ref_ptr<Scene> scene = dynamic_cast<Scene*>(node);
		if (scene.valid())
		{
			m_scene = scene.get();
			m_marker = scene->getMarker(m_markerName);
			if (!m_marker.valid())
			{
				Util::log(__FUNCTION__, "Cannot find marker", 2);
			}

		}
		else
		{
			Util::log(__FUNCTION__, "Invalid scene node", 2);
		}
	}

	// Found marker
	if (!m_markerName.empty()
		&& m_marker != NULL
		&& m_marker->getOsgMarker()->valid()
		&& m_objName.empty())
	{
		stringstream sstr;
		sstr << "marker " << m_markerName << " is visible";
		Util::log(__FUNCTION__, sstr.str().c_str(), 5);
		return true;
	}

	// Found obj on marker
	else if (!m_markerName.empty() 
		&& m_marker != NULL
		&& m_marker->getOsgMarker()->valid() 
		&& m_marker->getIdxForObjName(m_objName) != -1)
	{
		stringstream sstr;
		sstr << "obj " << m_objName << " is visible on marker " << m_markerName;
		Util::log(__FUNCTION__, sstr.str().c_str(), 5);
		return true;
	}
	// Found hud on scene
	else if (!m_objName.empty()
		&& m_scene->getHUDRoot()->getHUD(m_objName) != NULL)
	{
		stringstream sstr;
		sstr << "hud " << m_objName << " is visible";
		Util::log(__FUNCTION__, sstr.str().c_str(), 5);
		return true;
	}
	else 
	{
		stringstream sstr;
		sstr << "obj " << m_objName << " is not visible";
		Util::log(__FUNCTION__, sstr.str().c_str(), 5);
		return false;
	}
}