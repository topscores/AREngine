#include "arengine/Action.h"
#include "arengine/ActionFactory.h"

using namespace arengine;


Action::Action(DataNode *actionNode)
:m_enable(true)
{
	m_name = actionNode->getAttributeAsString("name");

	int preActionCount = actionNode->countChild("PreActionCallback");
	for (int i = 0;i < preActionCount;i++)
	{
		DataNode *callback = actionNode->getChild("PreActionCallback", i);
		addPreActionCB(ActionCallbackFactory::newCallback(callback));
	}

	int postActionCount = actionNode->countChild("PostActionCallback");
	for (int i = 0;i < preActionCount;i++)
	{
		DataNode *callback = actionNode->getChild("PostActionCallback", i);
		addPostActionCB(ActionCallbackFactory::newCallback(callback));
	}
}

Action::~Action()
{
}


void 
Action::exec(osg::Node *node)
{
	if (isEnable())
	{
		doPreAction(node);
		doAction(node);
		doPostAction(node);
	}
}


string
Action::getName()
{
	return m_name;
}


bool
Action::isEnable()
{
	return m_enable;
}


void 
Action::addPreActionCB(ActionCallback *callback)
{
	m_preActionCB.push_back(callback);
}


void 
Action::addPostActionCB(ActionCallback *callback)
{
	m_postActionCB.push_back(callback);
}


void 
Action::doPreAction(osg::Node *node)
{
	int n = m_preActionCB.size();
	for (int i = 0;i < n;i++)
	{
		m_preActionCB.at(i)->exec(node);
	}
}

void 
Action::doPostAction(osg::Node *node)
{
	int n = m_postActionCB.size();
	for (int i = 0;i < n;i++)
	{
		m_postActionCB.at(i)->exec(node);
	}
}


