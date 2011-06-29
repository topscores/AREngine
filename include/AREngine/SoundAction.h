#ifndef __SOUND_ACTION_H__
#define __SOUND_ACTION_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"

#include <osg/Node>

namespace arengine
{

	class ARENGINE_EXPORT StartSound : public Action
	{
	public:
		StartSound(DataNode *soundNode);
		~StartSound();

		virtual void doAction(osg::Node *node);
	};

	class ARENGINE_EXPORT StopSound : public Action
	{
	public:
		StopSound(DataNode *soundNode);
		~StopSound();

		virtual void doAction(osg::Node *node);
	};

}

#endif