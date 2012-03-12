#ifndef __SOUND_ACTION_H__
#define __SOUND_ACTION_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"
#include "arengine/ActionSet.h"

#include <osg/Node>
using namespace osg;

#include <string>
using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT StartSoundAction : public Action
	{
	public:
		StartSoundAction(DataNode *soundNode);
		~StartSoundAction();
		
		virtual void doAction(osg::Node *node);

	private:
		string m_soundName;
		bool m_loop;
	};

	class ARENGINE_EXPORT StopSoundAction : public Action
	{
	public:
		StopSoundAction(DataNode *soundNode);
		~StopSoundAction();

		virtual void doAction(osg::Node *node);

	private:
		string	m_soundName;
		string	m_pauseType;
		bool	m_stop;
	};

	class ARENGINE_EXPORT ToggleMuteAction : public Action
	{
	public:
		ToggleMuteAction(DataNode *soundNode)
			:Action(soundNode)
		{
		}

		~ToggleMuteAction(){}

		virtual void doAction(osg::Node *node);
	};

}

#endif