#ifndef __EXEC_PENDING_ACTION_CALLBACK__
#define __EXEC_PENDING_ACTION_CALLBACK__

#include "arengine/Export"
#include "arengine/Action.h"

#include <osg/NodeCallback>

#include <queue>
using namespace std;

namespace arengine
{
	class ARENGINE_EXPORT PendingActionCallback : public osg::NodeCallback
	{
	public:
		PendingActionCallback();
		~PendingActionCallback();

		void addPendingAction(Action *action);
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

	private:
		queue< ref_ptr<Action> > m_pendingAction;

		int m_checkInterval;
		int m_lastCheckTime;
	};
}

#endif 