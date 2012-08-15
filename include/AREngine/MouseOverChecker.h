#ifndef __MOUSEOVER_CHECKER_H__
#define __MOUSEOVER_CHECKER_H__

#include "arengine/Export"
#include "arengine/ConditionalCallback.h"
#include "arengine/DataNode.h"
#include "arengine/MouseHandler.h"

#include <osg/Node>
using namespace osg;

namespace arengine
{
	class MouseOverChecker : public ConditionChecker
	{
	public:
		MouseOverChecker(DataNode *checkerNode);
		~MouseOverChecker();

		virtual bool conditionValid(osg::Node *node);

	private:
		string m_objName;
		ref_ptr<MouseHandler> m_mh;
	};
}

#endif