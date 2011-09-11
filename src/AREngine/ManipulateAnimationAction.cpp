#include "arengine/ManipulateAnimationAction.h"
#include "arengine/Util.h"
#include "arengine/SceneObj.h"
#include "arengine/Scene.h"
#include "arengine/ObjPool.h"
#include "arengine/Singleton.h"

#include <osg/NodeCallback>
#include <osg/AnimationPath>

using namespace arengine;


ManipulateSequenceVisitor::ManipulateSequenceVisitor(string manipulateType)
:NodeVisitor(NodeVisitor::UPDATE_VISITOR, NodeVisitor::TRAVERSE_ALL_CHILDREN)
,m_manipulateType(manipulateType)
{
	if (m_manipulateType != "start"
		&& m_manipulateType != "stop"
		&& m_manipulateType != "pause"
		&& m_manipulateType != "resume")
	{
		Util::log(__FUNCTION__, 2, "Unsupport manipulationType:\"%s\"", m_manipulateType.c_str());
	}
}


void
ManipulateSequenceVisitor::apply(osg::Sequence &seq)
{
	if (m_manipulateType == "start")
	{
		seq.setMode(osg::Sequence::START);
	}
	else if (m_manipulateType == "stop")
	{
		seq.setMode(osg::Sequence::STOP);
	}
	else if (m_manipulateType == "pause")
	{
		seq.setMode(osg::Sequence::PAUSE);
	}
	else if (m_manipulateType == "resume")
	{
		seq.setMode(osg::Sequence::RESUME);
	}
}


ManipulateAnimationPathVisitor::ManipulateAnimationPathVisitor(string manipulateType)
:NodeVisitor(NodeVisitor::UPDATE_VISITOR, NodeVisitor::TRAVERSE_ALL_CHILDREN)
,m_manipulateType(manipulateType)
{
	if (m_manipulateType != "start"
		&& m_manipulateType != "stop"
		&& m_manipulateType != "pause"
		&& m_manipulateType != "resume")
	{
		Util::log(__FUNCTION__, 2, "Unsupport manipulationType:\"%s\"", m_manipulateType.c_str());
	}
}


void
ManipulateAnimationPathVisitor::apply(osg::Node &node)
{
	osg::NodeCallback *cb = node.getUpdateCallback();
	osg::AnimationPathCallback *acb = dynamic_cast<osg::AnimationPathCallback*>(cb);

	while (cb)
	{
		if (acb)
		{
			if (m_manipulateType == "start")
			{
				acb->reset();
				acb->setPause(false);
			}
			else if (m_manipulateType == "stop")
			{
				acb->reset();
				acb->setPause(true);
			}
			else if (m_manipulateType == "pause")
			{
				acb->setPause(true);
			}
			else if (m_manipulateType == "resume")
			{
				acb->setPause(false);
			}
		}
		cb = cb->getNestedCallback();
		acb = dynamic_cast<osg::AnimationPathCallback*>(cb);
	}

	traverse(node);
}


ManipulateAnimationAction::ManipulateAnimationAction(arengine::DataNode *datNode)
:Action(datNode)
{
	if (datNode->getNodeName() == "ManipulateAnimation")
	{
		m_markerName	= datNode->getAttributeAsString("markerName");
		m_objName		= datNode->getAttributeAsString("objName");
		m_manipulateType= datNode->getAttributeAsString("type");
	}
	else
	{
		Util::log(__FUNCTION__, "Not a ManipulateAnimation tag", 2);
	}
}


void
ManipulateAnimationAction::doAction(osg::Node *node)
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
				manipulateAnim(obj.get());
				Util::log(__FUNCTION__, 3, "Manipulate animation on %s with command \"%s\"", m_objName.c_str(), m_manipulateType.c_str());
			}
		}
		// Manipulate all active animation on marker
		else
		{
			int n = m_marker->countAssociatedObj();
			for (int i = 0;i < n;i++)
			{
				manipulateAnim((m_marker->getAssociatedObj(i)).get());
			}
			Util::log(__FUNCTION__, 3, "Manipulate animation on marker %s with command \"%s\"", m_markerName.c_str(), m_manipulateType.c_str());
		}
	}
	// Reset animation on specific obj
	else
	{
		SceneObjPool *pool = Singleton<SceneObjPool>::getInstance();
		ref_ptr<SceneObj> obj = pool->getByName(m_objName);
		if (obj.valid())
		{
			manipulateAnim(obj.get());
			Util::log(__FUNCTION__, 3, "Manipulate animation on %s with command \"%s\"", m_objName.c_str(), m_manipulateType.c_str());
		}
	}
}


void
ManipulateAnimationAction::manipulateAnim(SceneObj *obj)
{
	if (obj)
	{
		/*if (m_manipulateType == "stop")
		{
			obj->accept(ManipulateSequenceVisitor("start"));
			obj->accept(ManipulateAnimationPathVisitor("start"));

			obj->accept(ManipulateSequenceVisitor("stop"));
			obj->accept(ManipulateAnimationPathVisitor("stop"));
		}
		else
		{*/
        ManipulateSequenceVisitor msv(m_manipulateType);
        ManipulateAnimationPathVisitor mapv(m_manipulateType);
			obj->accept(msv);
			obj->accept(mapv);
		//}
	}
}