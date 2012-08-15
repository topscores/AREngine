#include "arengine/ActionFactory.h"

#include "arengine/Action.h"
#include "arengine/AddRemoveAction.h"
#include "arengine/SoundAction.h"
#include "arengine/ManipulateAnimationAction.h"
#include "arengine/ManipulateTextureAnimationAction.h"
#include "arengine/ManipulateMovieAction.h"
#include "arengine/ActionSet.h"
#include "arengine/CircularActionSet.h"
#include "arengine/TimerActionSet.h"
#include "arengine/DataNode.h"
#include "arengine/Util.h"
#include "arengine/TransitionAction.h"
#include "arengine/ChangeSceneAction.h"

#include <osg/Node>

using namespace arengine;
using namespace osg;


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
	else if (actionType == "ToggleMute")
	{
		return new ToggleMuteAction(action);
	}
	else if (actionType == "ManipulateAnimation")
	{
		return new ManipulateAnimationAction(action);
	}
	else if (actionType == "ManipulateTextureAnimation")
	{
		return new ManipulateTextureAnimationAction(action);
	}
	else if (actionType == "ManipulateMovie")
	{
		return new ManipulateMovieAction(action);
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
	else if (actionType == "ChangeScene")
	{
		return new ChangeSceneAction(action);
	}
	else
	{
		Util::log(__FUNCTION__, 2, "Unknown action\"%s\"", actionType.c_str());
		return NULL;
	}
}