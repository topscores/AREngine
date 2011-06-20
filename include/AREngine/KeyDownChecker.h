#ifndef __KEYDOWN_CHECKER_H__
#define __KEYDOWN_CHECKER_H__

#include "arengine/Export"
#include "arengine/ConditionalCallback.h"
#include "arengine/DataNode.h"
#include "arengine/KeyboardHandler.h"

#include <osg/Node>
using namespace osg;

namespace arengine
{
	class KeyDownChecker : public ConditionChecker
	{
	public:
		KeyDownChecker(DataNode *checkerNode);
		~KeyDownChecker();

		virtual bool conditionValid(osg::Node *node);

	private:
		int m_key;
		ref_ptr<KeyboardHandler> m_kbh;
	};
}

#endif