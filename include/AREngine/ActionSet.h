#ifndef __ACTION_SET_H__
#define __ACTION_SET_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"

#include <osg/Node>
using namespace osg;

#include <vector>
using namespace std;

namespace arengine
{
	// Execute all actions in set
	class ActionSet : public Action
	{
	public:
		ActionSet(DataNode *actionSetNode);
		~ActionSet();

		virtual void doAction(osg::Node *node);

		int actionCount();
		ref_ptr<Action> getChildAction(int id);

	protected:
		int m_actionCount;
		vector< ref_ptr<Action> > m_actions;
	};

}

#endif