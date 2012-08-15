#ifndef __CHANGE_SCENE_ACTION_H__
#define __CHANGE_SCENE_ACTION_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"

namespace arengine
{

	class ARENGINE_EXPORT ChangeSceneAction : public Action
	{
	public:
		ChangeSceneAction(DataNode *changeSceneNode);
		virtual void doAction(osg::Node *node);

	private:
		string m_sceneName;
	};

}

#endif