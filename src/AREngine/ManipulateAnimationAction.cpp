#include "arengine/ManipulateAnimationAction.h"
#include "arengine/Util.h"
#include "arengine/SceneObj.h"
#include "arengine/Scene.h"
#include "arengine/ObjPool.h"
#include "arengine/Singleton.h"
#include "arengine/Model.h"

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
	traverse(seq);
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
,m_changeSyncMode(false)
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
				manipulateAnim(node, obj.get());
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
		}
	}
}


void
ManipulateAnimationAction::manipulateAnim(Node *node, SceneObj *obj)
{
	if (obj && obj->getObjType() == E_MODEL)
	{
		if (m_manipulateType == "start" || m_manipulateType == "stop")
		{
			ManipulateSequenceVisitor msv(m_manipulateType);
			ManipulateAnimationPathVisitor mapv(m_manipulateType);
			obj->accept(msv);
			obj->accept(mapv);
		}
		else if (m_manipulateType == "pause")
		{
			if (!m_changeSyncMode)
			{
				ManipulateSequenceVisitor msv(m_manipulateType);
				ManipulateAnimationPathVisitor mapv(m_manipulateType);
				obj->accept(msv);
				obj->accept(mapv);

				// After pausing need to change sync mode to false to avoid frame skip after resuming
				m_changeSyncMode = true;
				addToPendingQueue(node, this);
			}
			// Change sync mode
			else
			{
				// After pausing need to change sync mode to false to avoid frame skip after resuming
				SetSyncModeVisitor ssmv(false);
				obj->accept(ssmv);
				m_changeSyncMode = false;
			}
		}
		else if (m_manipulateType == "resume")
		{
			if (!m_changeSyncMode)
			{
				ManipulateSequenceVisitor msv(m_manipulateType);
				ManipulateAnimationPathVisitor mapv(m_manipulateType);
				obj->accept(msv);
				obj->accept(mapv);

				// After resuming need to change sync mode to true to sync with timeline
				m_changeSyncMode = true;
				addToPendingQueue(node, this);
			}
			// Change sync mode
			else
			{
				// After resuming need to change sync mode to true to sync with timeline
				SetSyncModeVisitor ssmv(true);
				obj->accept(ssmv);
				m_changeSyncMode = false;
			}
		}
		Util::log(__FUNCTION__, 3, "Manipulate animation type=%s objName=%s", m_manipulateType.c_str(), m_objName.c_str());

	}
}