#include "arengine/ManipulateMovieAction.h"
#include "arengine/Util.h"
#include "arengine/SceneObj.h"
#include "arengine/Scene.h"
#include "arengine/ObjPool.h"
#include "arengine/Singleton.h"
#include "arengine/Movie.h"

using namespace arengine;

ManipulateMovieAction::ManipulateMovieAction(arengine::DataNode *datNode)
:Action(datNode)
{
	if (datNode->getNodeName() == "ManipulateMovie")
	{
		m_markerName	= datNode->getAttributeAsString("markerName");
		m_objName		= datNode->getAttributeAsString("objName");
		m_manipulateType= datNode->getAttributeAsString("type");
	}
	else
	{
		Util::log(__FUNCTION__, "Not a ManipulateMovie tag", 2);
	}
}


void
ManipulateMovieAction::doAction(osg::Node *node)
{
	ref_ptr<Scene> scene = dynamic_cast<Scene*>(node);
	if (!scene.valid())
	{
		Util::log(__FUNCTION__, "Invalid scene node", 2);
	}
	// Manipulate animation from obj on marker
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

		// Manipulate animation on specific marker
		if (!m_objName.empty())
		{
			ref_ptr<SceneObj> obj = m_marker->getAssociatedObj(m_objName);
			if (obj.valid())
			{
				manipulateAnim(node, obj.get());
				Util::log(__FUNCTION__, 3, "Manipulate movie on %s with command \"%s\"", m_objName.c_str(), m_manipulateType.c_str());
			}
			else 
			{
				Util::log(__FUNCTION__, 2, "Could not find object with name %s", m_objName.c_str());
			}
		}
		// Manipulate all active animation on marker
		else
		{
			int n = m_marker->countAssociatedObj();
			for (int i = 0;i < n;i++)
			{
				manipulateAnim(node, (m_marker->getAssociatedObj(i)).get());
			}
			Util::log(__FUNCTION__, 3, "Manipulate movie on marker %s with command \"%s\"", m_markerName.c_str(), m_manipulateType.c_str());
		}
	}
	// Reset animation on specific obj
	else
	{
		SceneObjPool *pool = Singleton<SceneObjPool>::getInstance();
		ref_ptr<SceneObj> obj = pool->getByName(m_objName);
		if (obj.valid())
		{
			manipulateAnim(node, obj.get());
			Util::log(__FUNCTION__, 3, "Manipulate movie on %s with command \"%s\"", m_objName.c_str(), m_manipulateType.c_str());
		}
	}
}


void
ManipulateMovieAction::manipulateAnim(Node *node, SceneObj *obj)
{
	if (obj && obj->getObjType() == E_MOVIE)
	{
		if (m_manipulateType == "start")
		{
			Movie *movie = (Movie*)obj;
			movie->start();
		}
		else if (m_manipulateType == "stop")
		{
			Movie *movie = (Movie*)obj;
			movie->stop();
		}
		else if (m_manipulateType == "pause")
		{
			Movie *movie = (Movie*)obj;
			movie->pause();
		}
		else if (m_manipulateType == "resume")
		{
			Movie *movie = (Movie*)obj;
			movie->resume();
		}
	}
}