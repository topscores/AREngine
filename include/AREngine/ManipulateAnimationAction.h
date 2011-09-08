#ifndef __MANIPULATE_ANIMATION_ACTION_H__
#define __MANIPULATE_ANIMATION_ACTION_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"
#include "arengine/Marker.h"

#include <osg/Node>
#include <osg/Sequence>


namespace arengine
{
	class ARENGINE_EXPORT ManipulateSequenceVisitor : public osg::NodeVisitor
	{
	public:
		ManipulateSequenceVisitor(string manipulateType);
		virtual void apply(osg::Sequence &seq);

	private:
		string m_manipulateType;
	};

	class ARENGINE_EXPORT ManipulateAnimationPathVisitor : public osg::NodeVisitor
	{
	public:
		ManipulateAnimationPathVisitor(string manipulateType);
		virtual void apply(osg::Node &node);

	private:
		string m_manipulateType;
	};

	class ARENGINE_EXPORT ManipulateAnimationAction : public Action
	{
	public:
		ManipulateAnimationAction(DataNode *datNode);
		virtual void doAction(osg::Node *node);

	private:
		void manipulateAnim(SceneObj *obj);

	private:
		string m_manipulateType;
		string m_markerName;
		string m_objName;

		ref_ptr<Marker> m_marker;
	};
}

#endif