#include "arengine/ActionSet.h"
#include "arengine/ActionFactory.h"
using namespace arengine;

ActionSet::ActionSet(DataNode *actionSetNode)
:Action(actionSetNode)
{
	m_actionCount = actionSetNode->getChildCount();
	for (int i = 0;i < m_actionCount;i++)
	{
		DataNode *action = actionSetNode->getChild(i);
		if (action)
		{
			m_actions.push_back(ActionFactory::newAction(action));
		}
	}
}


ActionSet::~ActionSet()
{
}


void
ActionSet::doAction(osg::Node *node)
{
	for (int i = 0;i < m_actionCount;i++)
	{
		m_actions.at(i)->exec(node);
	}
}


int
ActionSet::actionCount()
{
	return m_actions.size();
}


ref_ptr<Action>
ActionSet::getChildAction(int id)
{
	if (id >= 0 && id < m_actions.size())
	{
		return m_actions.at(id);
	}
	else
	{
		return NULL;
	}
}