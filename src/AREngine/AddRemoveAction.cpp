#include "arengine/AddRemoveAction.h"
#include "arengine/ARScene.h"
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
		m_layerid		= addNode->getAttributeAsInt("layer");
		m_target		= addNode->getAttributeAsString("target");
	}
	else
	{
		Util::log(__FUNCTION__, "Not an Add tag", 2);
	}
}


void
AddAction::doAction(osg::Node *node)
{
	if (!m_target.empty())
	{
		ref_ptr<ARScene> arscene = SmartSingleton<ARScene>::getInstance();
		ref_ptr<ARRoot> arroot = dynamic_cast<ARRoot*>(arscene->getSceneData().get());

		if (m_target == "allScene")
		{
			int n = arroot->getSceneCount();
			for (int i = 0;i < n;i++)
			{
				doAdd(arroot->getScene(i));
			}
		}
		else
		{
			ref_ptr<Scene> target = arroot->getScene(m_target);
			if (target.valid())
			{
				doAdd(target.get());
			}
			else
			{
				Util::log(__FUNCTION__, 2, "Cannot add %s to marker %s because scene with name =%s does not exist", m_objName, m_markerName, m_target);
			}
		}
	}
	else
	{
		doAdd(dynamic_cast<Scene*>(node));
	}
}


void
AddAction::doAdd(Scene *targetScene)
{
	ref_ptr<Scene> scene = targetScene;
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
			scene->getHUDRoot()->addHUD(m_objName, m_layerid);
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
		m_layerid		= removeNode->getAttributeAsInt("layer");
		m_target		= removeNode->getAttributeAsString("target");
	}
	else
	{
		Util::log(__FUNCTION__, "Not a Remove tag", 2);
	}
}

void
RemoveAction::doAction(osg::Node *node)
{
	if (!m_target.empty())
	{
		ref_ptr<ARScene> arscene = SmartSingleton<ARScene>::getInstance();
		ref_ptr<ARRoot> arroot = dynamic_cast<ARRoot*>(arscene->getSceneData().get());
		if (m_target == "allScene")
		{
			int n = arroot->getSceneCount();
			for (int i = 0;i < n;i++)
			{
				doRemove(arroot->getScene(i));
			}
		}
		else
		{
			ref_ptr<Scene> target = arroot->getScene(m_target);
			if (target.valid())
			{
				doRemove(target.get());
			}
			else
			{
				Util::log(__FUNCTION__, 2, "Cannot add %s to marker %s because scene with name =%s does not exist", m_objName, m_markerName, m_target);
			}
		}
	}
	else
	{
		doRemove(dynamic_cast<Scene*>(node));
	}
}


void
RemoveAction::doRemove(Scene *targetScene)
{
	ref_ptr<Scene> scene = targetScene;
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
				scene->getHUDRoot()->removeHUD(m_objName, m_layerid);
			}
			// otherwise remove all huds
			else
			{
				scene->getHUDRoot()->removeHUD(m_layerid);
			}
		}
		else
		{
			Util::log(__FUNCTION__, "HUDRoot is not initialized", 2);
		}
	}
}