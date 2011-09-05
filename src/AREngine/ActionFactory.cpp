#include "arengine/ActionFactory.h"

#include "arengine/Action.h"
#include "arengine/AddRemoveAction.h"
#include "arengine/SoundAction.h"
#include "arengine/ResetAnimationAction.h"
#include "arengine/ActionSet.h"
#include "arengine/CircularActionSet.h"
#include "arengine/TimerActionSet.h"
#include "arengine/DataNode.h"
#include "arengine/Util.h"
#include "arengine/TransitionAction.h"

#include <osg/Node>
#include <string>
#include <sstream>

using namespace arengine;
using namespace osg;
using namespace std;


ref_ptr<Action> 
ActionFactory::newAction(DataNode *action)
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
	else if (actionType == "StartSound")
	{
		return new StartSoundAction(action);
	}
	else if (actionType == "StopSound")
	{
		return new StopSoundAction(action);
	}
	else if (actionType == "ResetAnimation")
	{
		return new ResetAnimationAction(action);
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
	else if (actionType == "TwistIn")
	{
		return new TwistIn(action);
	}
	else
	{
		stringstream sstr;
		sstr << "Unknow action \"" << actionType << "\"";
		Util::log(__FUNCTION__, sstr.str().c_str(), 2);
	}
}