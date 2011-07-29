#include "arengine/PendingActionCallback.h"
#include "arengine/Config.h"
#include "arengine/Singleton.h"
#include "arengine/Util.h"

using namespace arengine;

PendingActionCallback::PendingActionCallback()
{
	// How often we should check the condition
	m_checkInterval = Singleton<Config>::getInstance()->getCheckInterval();
	m_lastCheckTime = Util::getElapseTimeInMilliSec();
}


PendingActionCallback::~PendingActionCallback()
{
}


void
PendingActionCallback::addPendingAction(Action *action)
{
	if (action != NULL)
	{
		m_pendingAction.push(action);
	}
	else
	{
		Util::log(__FUNCTION__, "Cannot add NULL action to queue", 2);
	}
}


void
PendingActionCallback::operator ()(osg::Node *node, osg::NodeVisitor *nv)
{
	int curTime = Util::getElapseTimeInMilliSec();
	if (curTime - m_lastCheckTime > m_checkInterval)
	{
		m_lastCheckTime = curTime;
		int pendingCount = m_pendingAction.size();
		for (int i = 0;i < pendingCount;i++)
		{
			ref_ptr<Action> action = m_pendingAction.front();
			m_pendingAction.pop();

			action->doAction(node);
		}
	}
	// must traverse the Node's subgraph   
	traverse(node, nv);
}
