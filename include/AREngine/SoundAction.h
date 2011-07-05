#ifndef __SOUND_ACTION_H__
#define __SOUND_ACTION_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"

#include <osg/Node>
#include <osgAudio/SoundState.h>
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
		
		ref_ptr<osgAudio::SoundState> getSoundState(){return m_soundState;}
		virtual void doAction(osg::Node *node);

	protected:
		ref_ptr<osgAudio::SoundState> createSoundStateFromSample();
		ref_ptr<osgAudio::SoundState> createSoundStateFromStream();

	private:
		string m_soundName;
		string m_soundType;
		bool m_loop;
		ref_ptr<osgAudio::SoundState> m_soundState;
	};

	class ARENGINE_EXPORT StopSound : public Action
	{
	public:
		StopSound(DataNode *soundNode);
		~StopSound();

		virtual void doAction(osg::Node *node);

	private:
		void doStop();

	private:
		string m_soundName;
		string m_pauseType;
		osgAudio::SourceState m_stopMethod;
		ref_ptr<osgAudio::SoundState> m_soundState;
	};

}

#endif