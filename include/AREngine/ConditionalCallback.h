#ifndef __CONDITIONAL_CALLBACK_H__
#define __CONDITIONAL_CALLBACK_H__

#include "arengine/Export"
#include "arengine/DataNode.h"
#include "arengine/Action.h"

#include <osg/Node>
#include <osg/NodeCallback>

#include <vector>

using namespace std;
using namespace osg;

namespace arengine
{
	enum ETriggerType
	{
		E_ON_VALID,
		E_ON_INVALID,
		E_ALWAYS
	};

	enum EConditionState
	{
		E_VALID,
		E_INVALID
	};

	class ARENGINE_EXPORT ConditionChecker : public osg::Referenced
	{
	public:
		virtual bool conditionValid(osg::Node *node) = 0;
	};

	class ARENGINE_EXPORT ConditionalCallback : public osg::NodeCallback
	{
	public:
		ConditionalCallback(DataNode *callbackNode);
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

	private:
		// True when we should raise action on node, False otherwise
		bool valid(osg::Node *node);
		bool allConditionValid(osg::Node *node);

		void processConditionData(DataNode *conditionNode);
		void processActionData(DataNode *actionNode);

	private:
		vector< ref_ptr<ConditionChecker> > m_checkers;
		vector< ref_ptr<Action> > m_actions;

		int m_checkInterval;
		int m_lastCheckTime;

		int m_validCount;
		int m_invalidCount;

		int m_validThresh;
		int m_invalidThresh;

		EConditionState m_lastState;
		ETriggerType m_triggerType;
	};
}

#endif