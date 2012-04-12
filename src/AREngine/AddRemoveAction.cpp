#include "arengine/AddRemoveAction.h"
#include "arengine/Util.h"
#include "arengine/Scene.h"
#include "arengine/HUDRoot.h"

using namespace arengine;

AddAction::AddAction(DataNode *addNode)
:Action(addNode)
{
	if (addNode->getNodeName() == "Add")
	{
		m_markerName	= addNode->getAttributeAsString("markerName");
		m_objName		= addNode->getAttributeAsString("objName");
	}
	else
	{
		Util::log(__FUNCTION__, "Not an Add tag", 2);
	}
}


void
AddAction::doAction(osg::Node *node)
{
	ref_ptr<Scene> scene = dynamic_cast<Scene*>(node);
	if (!scene.valid())
	{
		Util::log(__FUNCTION__, "Invalid scene node", 2);
	}
	// Add to one of the marker in scene
	if (!m_markerName.empty())
	{
		if (!m_marker.valid())
		{
			m_marker = scene->getMarker(m_markerName);
			if (!m_marker.valid())
			{
				Util::log(__FUNCTION__, "Cannot find marker", 2);
			}
		}

		m_marker->addAssociatedObj(m_objName);
	}
	// Add to hud in scene
	else
	{
		if (scene->getHUDRoot() != NULL)
		{
			scene->getHUDRoot()->addHUD(m_objName);
		}
		else
		{
			Util::log(__FUNCTION__, "HUDRoot is not initialized", 2);
		}
	}
}


RemoveAction::RemoveAction(DataNode *removeNode)
:Action(removeNode)
{
	if (removeNode->getNodeName() == "Remove")
	{
		m_markerName	= removeNode->getAttributeAsString("markerName");
		m_objName		= removeNode->getAttributeAsString("objName");
	}
	else
	{
		Util::log(__FUNCTION__, "Not a Remove tag", 2);
	}
}

void
RemoveAction::doAction(osg::Node *node)
{
	ref_ptr<Scene> scene = dynamic_cast<Scene*>(node);
	if (!scene.valid())
	{
		Util::log(__FUNCTION__, "Invalid scene node", 2);
		return;
	}

	// Remove from one of the marker in scene
	if (!m_markerName.empty())
	{
		if (!m_marker.valid())
		{
			m_marker = scene->getMarker(m_markerName);
			if (!m_marker.valid())
			{
				Util::log(__FUNCTION__, "Cannot find marker", 2);
			}
		}

		// If user provided m_objName remove that object
		if (!m_objName.empty())
		{
			m_marker->removeAssociatedObj(m_objName);
		}
		// Otherwise remove all associated obj
		else
		{
			m_marker->removeAssociatedObj();
		}
	}
	// Remove from hud
	else
	{
		if (scene->getHUDRoot() != NULL)
		{
			// If objName are provided remove it
			if (!m_objName.empty())
			{
				scene->getHUDRoot()->removeHUD(m_objName);
			}
			// otherwise remove all huds
			else
			{
				scene->getHUDRoot()->removeHUD();
			}
		}
		else
		{
			Util::log(__FUNCTION__, "HUDRoot is not initialized", 2);
		}
	}
}