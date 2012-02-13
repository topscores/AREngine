#ifndef __ACTION_H__
#define __ACTION_H__

#include "arengine/Export"
#include "arengine/NamedObj.h"
#include "arengine/DataNode.h"
#include "arengine/ActionCallback.h"
#include <osg/Node>

#include <string>
#include <vector>

using namespace std;
using namespace osg;

namespace arengine
{

	class ARENGINE_EXPORT Action : public Referenced, public arengine::NamedObj
	{
	public:
		Action(DataNode *actionNode);
		~Action();

		virtual	string getName();

		bool isEnable();
		virtual void exec(osg::Node *node);

		virtual void addPreActionCB(ActionCallback *callback);
		virtual void addPostActionCB(ActionCallback *callback);

		virtual void doPreAction(osg::Node *node);
		virtual void doPostAction(osg::Node *node);

		virtual void doAction(osg::Node *node) = 0;

	protected:
		virtual void addToPendingQueue(osg::Node *node, Action *action);

	protected:
		string m_name;
		vector< ref_ptr<ActionCallback> > m_preActionCB;
		vector< ref_ptr<ActionCallback> > m_postActionCB;
		bool m_enable;
	};

}

#endif