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

	class ARENGINE_EXPORT StartSound : public Action
	{
	public:
		StartSound(DataNode *soundNode);
		~StartSound();
		
		virtual void doAction(osg::Node *node);

	private:
		string m_soundName;
		bool m_loop;
	};

	class ARENGINE_EXPORT StopSound : public Action
	{
	public:
		StopSound(DataNode *soundNode);
		~StopSound();

		virtual void doAction(osg::Node *node);

	private:
		string	m_soundName;
		string	m_pauseType;
		bool	m_stop;
	};

}

#endif