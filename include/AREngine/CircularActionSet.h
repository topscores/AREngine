#ifndef __CIRCULAR_ACTION_SET_H__
#define __CIRCULAR_ACTION_SET_H__

#include "arengine/Export"
#include "arengine/ActionSet.h"
#include "arengine/DataNode.h"

#include <osg/Node>
#include <vector>

using namespace osg;
using namespace std;

namespace arengine
{

	// Execute an action in set everytime doAction is called and advance curIndex to next action in set
	class ARENGINE_EXPORT CircularActionSet : public ActionSet
	{
	public:
		CircularActionSet(DataNode *actionSetNode);
		~CircularActionSet();

		virtual void doAction(osg::Node *node);
		virtual void reset();

	protected:
		int m_curAction;

	};

}

#endif