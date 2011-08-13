#include "arengine/ConditionalCallback.h"
#include "arengine/CollisionChecker.h"
#include "arengine/KeyDownChecker.h"
#include "arengine/AppearChecker.h"
#include "arengine/Util.h"
#include "arengine/ActionFactory.h"
#include "arengine/Singleton.h"
#include "arengine/ObjPool.h"
#include "arengine/Config.h"

#include <string>

using namespace std;
using namespace arengine;

ConditionalCallback::ConditionalCallback(DataNode *callbackNode):
m_validCount(0),
m_invalidCount(0),
m_lastState(E_INVALID)
{
	if (callbackNode->getNodeName().compare("ConditionalCallback") == 0)
	{
		// How often we should check the condition
		m_checkInterval = Singleton<Config>::getInstance()->getCheckInterval();
		m_lastCheckTime = Util::getElapseTimeInMilliSec();

		// How many times an event should be detected before declared  valid
		int thresh = callbackNode->getAttributeAsInt("threshold");
		m_validThresh = thresh;
		m_invalidThresh = thresh;

		// When to trigger action
		string type = callbackNode->getAttributeAsString("type");
		if (type.compare("onValid") == 0)
		{
			m_triggerType = E_ON_VALID;
		}
		else if (type.compare("onInvalid") == 0)
		{
			m_triggerType = E_ON_INVALID;
		}
		else if (type.compare("always") == 0)
		{
			m_triggerType = E_ALWAYS;
		}
		// Default case
		else
		{
			m_triggerType = E_ON_VALID;
		}

		// Process all conditions
		DataNode *conditionNode = callbackNode->getChild("Condition", 0);
		if (conditionNode)
		{
			processConditionData(conditionNode);
		}

		// Process all actions
		DataNode *actionNode = callbackNode->getChild("Action", 0);
		if (actionNode)
		{
			processActionData(actionNode);
		}
	}
	else
	{
		Util::log(__FUNCTION__, "Not a ConditionalCallback tag", 2);
	}
}


void
ConditionalCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	int curTime = Util::getElapseTimeInMilliSec();

	if ((curTime - m_lastCheckTime) > m_checkInterval)
	{
		m_lastCheckTime = curTime;
		if (valid(node))
		{
			vector< ref_ptr<Action> >::iterator actionItr;
			for (actionItr = m_actions.begin();actionItr != m_actions.end();actionItr++)
			{
				ref_ptr<Action> action = *actionItr;
				action->exec(node);
			}
		}
	}

	// must traverse the Node's subgraph            
	traverse(node,nv);
}


bool
ConditionalCallback::valid(osg::Node *node)
{
	bool valid = allConditionValid(node);;
	switch (m_triggerType)
	{
	case E_ON_VALID:
		// Wait for "threshold" consecutive invalid before declaring invalid event
		if (m_lastState == E_VALID)
		{
			if (!valid)
			{
				if (m_invalidCount > m_invalidThresh)
				{
					m_lastState = E_INVALID;
					m_validCount = 0;
				}
				else
				{
					m_invalidCount++;
				}
			}
			else
			{
				m_invalidCount = 0;
			}
			valid = false;
		}
		else
		{
			// Wait for "threshold" consecutive valid before declaring valid event
			if (valid)
			{
				if (m_validCount > m_validThresh)
				{
					valid = true;
					stringstream sstr;
					sstr << "validCount = " << m_validCount << "," << "validThresh = " << m_validThresh;
					Util::log(sstr.str().c_str(), 5);
					m_lastState = E_VALID;
					m_invalidCount = 0;
				}
				else
				{
					valid = false;
					m_validCount++;
					stringstream sstr;
					sstr << "validCount = " << m_validCount;
					Util::log(sstr.str().c_str(), 5);
				}
			}
			else
			{
				valid = false;
				m_validCount = 0;
			}
		}
		break;
	case E_ON_INVALID:
		// Wait for "threshold" consecutive valid before declaring valid event
		if (m_lastState == E_INVALID)
		{
			if (valid)
			{
				if (m_validCount > m_validThresh)
				{
					m_lastState = E_VALID;
					m_invalidCount = 0;
				}
				else
				{
					m_validCount++;
				}
			}
			else
			{
				m_validCount = 0;
			}
			valid = false;
		}
		// Wait for "threshold" consecutive invalid before declaring invalid event
		else
		{
			if (!valid)
			{
				if (m_invalidCount > m_invalidThresh)
				{
					valid = true;

					m_lastState = E_INVALID;
					m_validCount = 0;
				}
				else
				{
					valid =false;
					m_invalidCount++;
				}
			}
			else
			{
				valid = false;
				m_invalidCount = 0;
			}
		}
		break;
	case E_ALWAYS:
		break;
	}

	return valid;
}


bool
ConditionalCallback::allConditionValid(osg::Node *node)
{
	// No condition, just behave like an update callback
	if (m_checkers.size() == 0)
	{
		Util::log(__FUNCTION__, "There is no condition register", 5);
		return true;
	}

	vector< ref_ptr<ConditionChecker> >::iterator checkerItr;
	for (checkerItr = m_checkers.begin();checkerItr != m_checkers.end();checkerItr++)
	{
		// Exit if one of the conditions is not satisfied
		ref_ptr<ConditionChecker> checker = *checkerItr;
		if (!checker->conditionValid(node))
		{
			return false;
		}
	}
	return true;
}


void
ConditionalCallback::processConditionData(DataNode *conditionNode)
{
	if (conditionNode->getNodeName().compare("Condition") == 0)
	{
		int checkerCount = conditionNode->getChildCount();
		for (int i = 0;i < checkerCount;i++)
		{
			DataNode *checker = conditionNode->getChild(i);
			if (checker)
			{
				string type = checker->getNodeName();
				if (type.compare("Collision") == 0)
				{
					m_checkers.push_back(new CollisionChecker(checker));
				}
				else if (type.compare("KeyDown") == 0)
				{
					m_checkers.push_back(new KeyDownChecker(checker));
				}
				else if (type.compare("Appear") == 0)
				{
					m_checkers.push_back(new AppearChecker(checker));
				}
				else
				{
					stringstream sstr;
					sstr << "Unknown checker \"" << type << "\"";
					Util::log(__FUNCTION__, sstr.str().c_str(), 2);
				}
			}
		}

	}
	else
	{
		Util::log("ConditionalCallback::processConditionData() : Not a Condition tag", 2);
	}
}


void
ConditionalCallback::processActionData(DataNode *actionNode)
{
	if (actionNode->getNodeName().compare("Action") == 0)
	{
		int actionCount = actionNode->getChildCount();
		for (int i = 0;i < actionCount;i++)
		{
			DataNode *action = actionNode->getChild(i);
			if (action)
			{
				ref_ptr<Action> tmp = ActionFactory::newAction(action);
				if (tmp.valid())
				{
					ActionPool *pool = Singleton<ActionPool>::getInstance();
					pool->addObj(tmp.get());
					m_actions.push_back(tmp);
				}
			}
		}

	}
	else
	{
		Util::log("ConditionalCallback::processActionData() : Not an Action tag", 2);
	}
}