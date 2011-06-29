#ifndef __ACTION_FACTORY_H__
#define __ACTION_FACTORY_H__

#include "arengine/Action.h"
#include "arengine/AddRemoveAction.h"
#include "arengine/ActionSet.h"
#include "arengine/CircularActionSet.h"
#include "arengine/TimerActionSet.h"
#include "arengine/DataNode.h"
#include "arengine/Util.h"

#include <osg/Node>
#include <string>
#include <sstream>

using namespace osg;
using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT ActionCallbackFactory
	{
	public:
		static ref_ptr<ActionCallback> newCallback(DataNode *callback)
		{
			if (callback == NULL)
				return NULL;

			string type = callback->getAttributeAsString("type");
			if (type == "ScreenCapture")
			{
				//return new ScreenCapture();
				return NULL;
			}
			else
			{
				return NULL;
			}


		}
	};

	class ARENGINE_EXPORT ActionFactory
	{
	public:
		static ref_ptr<Action> newAction(DataNode *action)
		{
			if (action == NULL)
				return NULL;

			string actionType = action->getNodeName();
			if (actionType == "Add")
			{
				return new AddAction(action);
			}
			else if (actionType == "Remove")
			{
				return new RemoveAction(action);
			}
			else if (actionType == "ActionSet")
			{
				return new ActionSet(action);
			}
			else if (actionType == "CircularActionSet")
			{
				return new CircularActionSet(action);
			}
			else if (actionType == "TimerActionSet")
			{
				return new TimerActionSet(action);
			}
			else
			{
				stringstream sstr;
				sstr << "Unknow action \" " << actionType << "\"";
				Util::log(__FUNCTION__, sstr.str().c_str(), 2);
			}
		}
	};
}

#endif