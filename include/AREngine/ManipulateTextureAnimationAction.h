#ifndef __MANIPULATE_TEXTURE_ANIMATION__
#define __MANIPULATE_TEXTURE_ANIMATION__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"
#include "arengine/SceneObj.h"

#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/ImageStream>

namespace arengine
{
	class ARENGINE_EXPORT ManipulateTextureAnimationVisitor : public osg::NodeVisitor
	{
	public:
		ManipulateTextureAnimationVisitor(string manipulateType);
		virtual void apply(osg::Geode &node);

	private:
		string m_manipulateType;
	};


	class ARENGINE_EXPORT ManipulateTextureAnimationAction : public Action
	{
	public:
		ManipulateTextureAnimationAction(DataNode *datNode);
		virtual void doAction(osg::Node *node);

	private:
		string m_manipulateType;
		string m_objName;
		ref_ptr<SceneObj> m_obj;
	};
}

#endif