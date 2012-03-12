#ifndef __MOUSEDOWN_CHECKER_H__
#define __MOUSEDOWN_CHECKER_H__

#include "arengine/Export"
#include "arengine/ConditionalCallback.h"
#include "arengine/DataNode.h"
#include "arengine/MouseHandler.h"

#include <osg/Node>
using namespace osg;

namespace arengine
{
	class MouseDownChecker : public ConditionChecker
	{
	public:
		MouseDownChecker(DataNode *checkerNode);
		~MouseDownChecker();

		virtual bool conditionValid(osg::Node *node);

	private:
		string m_objName;
		ref_ptr<MouseHandler> m_mh;
	};
}

#endif