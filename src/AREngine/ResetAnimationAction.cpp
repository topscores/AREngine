#include "arengine/ResetAnimationAction.h"
#include "arengine/Util.h"
#include "arengine/SceneObj.h"
#include "arengine/Scene.h"
#include "arengine/ObjPool.h"
#include "arengine/Singleton.h"

#include <osg/NodeCallback>
#include <osg/AnimationPath>

using namespace arengine;


ResetSequenceVisitor::ResetSequenceVisitor()
:NodeVisitor(NodeVisitor::UPDATE_VISITOR, NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
}


void
ResetSequenceVisitor::apply(osg::Sequence &seq)
{
	seq.setMode(osg::Sequence::START);
}


ResetAnimationPathVisitor::ResetAnimationPathVisitor()
:NodeVisitor(NodeVisitor::UPDATE_VISITOR, NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
}


void
ResetAnimationPathVisitor::apply(osg::Node &node)
{
	osg::NodeCallback *cb = node.getUpdateCallback();
	osg::AnimationPathCallback *acb = dynamic_cast<osg::AnimationPathCallback*>(cb);

	while (cb)
	{
		if (acb)
		{
			acb->reset();
		}
		cb = cb->getNestedCallback();
		acb = dynamic_cast<osg::AnimationPathCallback*>(cb);
	}

	traverse(node);
}


ResetAnimationAction::ResetAnimationAction(arengine::DataNode *resetNode)
:Action(resetNode)
{
	if (resetNode->getNodeName() == "ResetAnimation")
	{
		m_markerName	= resetNode->getAttributeAsString("markerName");
		m_objName		= resetNode->getAttributeAsString("objName");
	}
	else
	{
		Util::log(__FUNCTION__, "Not a ResetAnimation tag", 2);
	}
}


void
ResetAnimationAction::doAction(osg::Node *node)
{
	ref_ptr<Scene> scene = dynamic_cast<Scene*>(node);
	if (!scene.valid())
	{
		Util::log(__FUNCTION__, "Invalid scene node", 2);
	}
	// Reset animation from obj on marker
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

		// Reset animation on specific marker
		if (!m_objName.empty())
		{
			ref_ptr<SceneObj> obj = m_marker->getAssociatedObj(m_objName);
			resetAnim(obj.get());
		}
		// Reset all active animation on marker
		else
		{
			int n = m_marker->countAssociatedObj();
			for (int i = 0;i < n;i++)
			{
				resetAnim((m_marker->getAssociatedObj(i)).get());
			}
		}
	}
	// Reset animation on specific obj
	else
	{
		SceneObjPool *pool = Singleton<SceneObjPool>::getInstance();
		ref_ptr<SceneObj> obj = pool->getByName(m_objName);
		resetAnim(obj.get());
	}
}


void
ResetAnimationAction::resetAnim(SceneObj *obj)
{
	if (obj)
	{
		obj->accept(ResetSequenceVisitor());
		obj->accept(ResetAnimationPathVisitor());
	}
}