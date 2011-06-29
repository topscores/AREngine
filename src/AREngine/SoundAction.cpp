#include "arengine/SoundAction.h"
#include "arengine/Util.h"

#include <osgAudio/SoundManager.h>

#include <sstream>

using namespace std;
using namespace arengine;

StartSound::StartSound(DataNode *soundNode)
:Action(soundNode)
{
	if (soundNode->getNodeName() == "StartSound")
	{
		m_soundName	= soundNode->getAttributeAsString("soundName");
		m_soundType	= m_soundName.substr(m_soundName.find_last_of(".") + 1);
		// Default is no loop
		m_loop	=	soundNode->getAttributeAsBool("loop");

		if (m_soundType == "wav")
		{
			m_soundState = createSoundStateFromSample();
		}
		else if (m_soundType == "ogg")
		{
			m_soundState = createSoundStateFromStream();
		}
		else
		{
			m_soundState == NULL;

			stringstream sstr;
			sstr << "Unsupported extension \"" << m_soundType << "\"";
			Util::log(__FUNCTION__, sstr.str().c_str(), 2);
		}
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
	if (m_soundState && (!m_soundState->getPlay()) )
	{
		m_soundState->setPlay(true);
	}
	else
	{
		Util::log(__FUNCTION__, "No registered soundState", 2);
	}
}


ref_ptr<osgAudio::SoundState>
StartSound::createSoundStateFromSample()
{
	osgAudio::SoundState *soundState;
	if (!m_soundState)
	{
		// If not found, create a new one
		soundState = new osgAudio::SoundState(m_soundName);
		// ALlocate a hw source so we can loop it
		soundState->allocateSource( 10 );

		// Create a new filestream that streams samples from a ogg-file.
		osgAudio::Sample *sample = new osgAudio::Sample(m_soundName);
		if (sample)
		{
			// Associate the stream with the sound state
			soundState->setSample( sample );
			// Make it an ambient (heard everywhere) sound
			soundState->setAmbient( true );
			// Loop the sound forever
			soundState->setLooping( m_loop );
			// Start playing the music!
			soundState->setPlay( false );

			// Add the soundstate to the sound manager so we can find it later on.
			osgAudio::SoundManager::instance()->addSoundState( soundState );

			stringstream sstr;
			sstr << "Successfully create sound state for sample " << m_soundName;
			Util::log(__FUNCTION__, sstr.str().c_str(), 3);
		}
		else
		{
			stringstream sstr;
			sstr << "Unable to create sound state for sample " << m_soundName;
			Util::log(__FUNCTION__, sstr.str().c_str(), 2);
		}
	}
	return soundState;
}


ref_ptr<osgAudio::SoundState>
StartSound::createSoundStateFromStream()
{
	osgAudio::SoundState *soundState;
	if (!m_soundState)
	{
		// If not found, create a new one
		soundState = new osgAudio::SoundState(m_soundName);
		// ALlocate a hw source so we can loop it
		soundState->allocateSource( 10 );

		// Create a new filestream that streams samples from a ogg-file.
		osgAudio::FileStream *stream = new osgAudio::FileStream(m_soundName);
		if (stream)
		{
			// Associate the stream with the sound state
			soundState->setStream( stream );
			// Make it an ambient (heard everywhere) sound
			soundState->setAmbient( true );
			// Loop the sound forever
			soundState->setLooping( m_loop );
			// Start playing the music!
			soundState->setPlay( false );

			// Add the soundstate to the sound manager so we can find it later on.
			osgAudio::SoundManager::instance()->addSoundState( soundState );

			stringstream sstr;
			sstr << "Successfully create sound state for stream " << m_soundName;
			Util::log(__FUNCTION__, sstr.str().c_str(), 3);
		}
		else
		{
			stringstream sstr;
			sstr << "Unable to create sound state for stream " << m_soundName;
			Util::log(__FUNCTION__, sstr.str().c_str(), 2);
		}
	}
	return soundState;
}

//==================================
StopSound::StopSound(DataNode *soundNode)
:Action(soundNode)
{
	if (soundNode->getNodeName() == "StopSound")
	{
		m_soundName	= soundNode->getAttributeAsString("soundName");
		m_pauseType	= soundNode->getAttributeAsString("pauseType");
		if (m_pauseType.empty())
		{
			m_pauseType = "stop";
		}
		
		m_soundState = osgAudio::SoundManager::instance()->findSoundState(m_soundName);
		if (!m_soundState)
		{
			if (m_pauseType == "stop")
			{
				m_soundState->setStopMethod(osgAudio::Stopped);
			}
			else
			{
				m_soundState->setStopMethod(osgAudio::Paused);
			}
		}
		else
		{
			stringstream sstr;
			sstr << "Cannot find registered soundState with name " << m_soundName;
			Util::log(__FUNCTION__, sstr.str().c_str(), 2);
		}
			
	}
	else
	{
		Util::log(__FUNCTION__, "Not a StartSound tag", 2);
	}
}


StopSound::~StopSound()
{
}


void
StopSound::doAction(osg::Node *node)
{
	if (m_soundState)
	{
		m_soundState->setPlay(false);
	}
	else
	{
		m_soundState = osgAudio::SoundManager::instance()->findSoundState(m_soundName);
		if (m_soundState)
		{
			m_soundState->setPlay(false);
		}
	}
}