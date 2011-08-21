#include "arengine/SoundAction.h"
#include "arengine/Util.h"
#include "arengine/Singleton.h"
#include "arengine/SDLSoundManager.h"

using namespace std;
using namespace arengine;

StartSound::StartSound(DataNode *soundNode)
:Action(soundNode)
{
	if (soundNode->getNodeName() == "StartSound")
	{
		m_soundName	= soundNode->getAttributeAsString("soundName");
		
		// Default is no loop
		m_loop	=	soundNode->getAttributeAsBool("loop");
	}
	else
	{
		Util::log(__FUNCTION__, "Not a StartSound tag", 2);
	}
}


StartSound::~StartSound()
{
}


void
StartSound::doAction(osg::Node *node)
{
	SDLSoundManager *soundMgr = Singleton<SDLSoundManager>::getInstance();
	soundMgr->play(m_soundName, m_loop);
}

//==================================
StopSound::StopSound(DataNode *soundNode)
:Action(soundNode)
{
	if (soundNode->getNodeName() == "StopSound")
	{
		m_soundName	= soundNode->getAttributeAsString("soundName");
		
		string pauseType	= soundNode->getAttributeAsString("pauseType");
		if (pauseType.empty())
		{
			m_stop = true;
		}
		else if (pauseType == "stop")
		{
			m_stop = true;
		}
		else
		{
			// pause instead of halt
			m_stop = false;
		}
	}
	else
	{
		Util::log(__FUNCTION__, "Not a StopSound tag", 2);
	}
}


StopSound::~StopSound()
{
}


void
StopSound::doAction(osg::Node *node)
{
	SDLSoundManager *soundMgr = Singleton<SDLSoundManager>::getInstance();

	// If m_soundName == "", will stop or pause all active audios
	if (m_stop)
	{
		soundMgr->stop(m_soundName);
	}
	else
	{
		soundMgr->pause(m_soundName);
	}
}