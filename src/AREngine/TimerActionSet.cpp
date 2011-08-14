#include "arengine/TimerActionSet.h"
#include "arengine/Util.h"
#include "arengine/ActionFactory.h"
#include "arengine/PendingActionCallback.h"
#include "arengine/Scene.h"

#define DEFAULT_SLEEP_TIME 1000
#define DEFAULT_SLEEP_COUNT 5

using namespace arengine;
using namespace std;

TimerActionSet::TimerActionSet(DataNode *actionSetNode)
:ActionSet(actionSetNode),
m_curSleepCount(-1),
m_lastExec(0)
{
	m_sleepTime = actionSetNode->getAttributeAsInt("sleepTime");
	if (m_sleepTime == 0)
	{
		m_sleepTime = DEFAULT_SLEEP_TIME;
		Util::log(__FUNCTION__,"sleepTime is not specified", 2);
	}

	m_sleepCount = actionSetNode->getAttributeAsInt("sleepCount");
	if (m_sleepCount == 0)
	{
		m_sleepCount = DEFAULT_SLEEP_COUNT;
		Util::log(__FUNCTION__,"sleepCount is not specified", 2);
	}
}



TimerActionSet::~TimerActionSet()
{
}


void
TimerActionSet::exec(osg::Node *node)
{
	if (isEnable())
	{
		doPreAction(node);
		m_lastExec = Util::getElapseTimeInMilliSec();
		m_curSleepCount = 0;

		addToPendingQueue(node, this);
	}
}


void
TimerActionSet::doPreAction(osg::Node *node)
{
	int n = m_preActionCB.size();
	for (int i = 0;i < n;i++)
	{
		m_preActionCB.at(i)->exec(node);
	}
	m_enable = false;
}


void
TimerActionSet::doPostAction(osg::Node *node)
{
	int n = m_postActionCB.size();
	for (int i = 0;i < n;i++)
	{
		m_postActionCB.at(i)->exec(node);
	}
	m_enable = true;
}


void
TimerActionSet::doAction(osg::Node *node)
{
	// Check whether elapse time from last execute exceed sleepTime
	int timeDiff = Util::getElapseTimeInMilliSec() - m_lastExec;
	if (timeDiff >= m_sleepTime)
	{
		if (m_curSleepCount < m_sleepCount)
		{
			for (int i = 0;i < m_actionCount;i++)
			{
				m_actions.at(i)->exec(node);
			}

			// Update last execute time
			m_lastExec = Util::getElapseTimeInMilliSec();
			m_curSleepCount++;

			stringstream sstr;
			sstr << "Execute action for " << m_curSleepCount << " times";
			Util::log(__FUNCTION__, sstr.str().c_str(), 4);

			// We have repeatly executed this action for sleepCount time, reset curSleepCount for next time call
			if (m_curSleepCount == m_sleepCount)
			{
				doPostAction(node);
				m_curSleepCount = -1;
				Util::log(__FUNCTION__, "Remove action to pending queue", 3);
			}
			else
			{
				addToPendingQueue(node, this);
			} // if m_curSleepCount == m_sleepCount
		} // if m_curSleepCount < m_sleepCount

	} // if timeDif
	else
	{
		addToPendingQueue(node, this);
	}
}