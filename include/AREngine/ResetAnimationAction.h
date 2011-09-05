#ifndef __RESET_ANIMATION_ACTION_H__
#define __RESET_ANIMATION_ACTION_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"
#include "arengine/Marker.h"

#include <osg/Node>
#include <osg/Sequence>


namespace arengine
{
	class ARENGINE_EXPORT ResetSequenceVisitor : public osg::NodeVisitor
	{
	public:
		ResetSequenceVisitor();
		virtual void apply(osg::Sequence &seq);
	};

	class ARENGINE_EXPORT ResetAnimationPathVisitor : public osg::NodeVisitor
	{
	public:
		ResetAnimationPathVisitor();
		virtual void apply(osg::Node &node);
	};

	class ARENGINE_EXPORT ResetAnimationAction : public Action
	{
	public:
		ResetAnimationAction(DataNode *resetNode);
		virtual void doAction(osg::Node *node);

	private:
		void resetAnim(SceneObj *obj);

	private:
		string m_markerName;
		string m_objName;

		ref_ptr<Marker> m_marker;
	};
}

#endif