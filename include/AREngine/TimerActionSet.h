#ifndef __TIMER_ACTION_SET_H__
#define __TIMER_ACTION_SET_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/ActionSet.h"
#include "arengine/DataNode.h"

#include <osg/Referenced>

using namespace osg;

namespace arengine
{

	// Execute all actions in set periodically 
	class ARENGINE_EXPORT TimerActionSet : public ActionSet
	{
	public:
		TimerActionSet(DataNode *actionNode);
		~TimerActionSet();

		virtual void exec(osg::Node *node);
		
		virtual void doPreAction(osg::Node *node);
		virtual void doPostAction(osg::Node *node);
		
		virtual void doAction(osg::Node *node);

	protected:
		int m_sleepTime;
		int m_sleepCount;
		int m_curSleepCount;
		int m_lastExec;

	};

}

#endif