#ifndef __TRANSITION_ACTION_H__
#define __TRANSITION_ACTION_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"
#include "arengine/SceneObj.h"

#include <osg/Vec3d>

namespace arengine
{

	class ARENGINE_EXPORT TwistIn : public Action
	{
	public:
		TwistIn(DataNode *transtionNode);
		~TwistIn();

		virtual void exec(osg::Node *node);
		virtual void doPreAction(osg::Node *node);
		virtual void doAction(osg::Node *node);
		virtual void doPostAction(osg::Node *node);

	private:
		SceneObj *m_obj;

		osg::Vec3d m_twistStep;
		osg::Vec3d m_startRotation;

		double m_scaleStep;
		double m_startScale;
		double m_curScale;

		osg::Vec3d m_orgRotation;
		double m_orgScale;

		int m_stepCount;
		int m_stepLast;
	};

}

#endif