#include "SDL.h"
#include "SDL_mixer.h"

#include "arengine/Util.h"
#include "arengine/AudioSample.h"

using namespace arengine;

WavSample::WavSample(string soundName)
:Sample(soundName),
m_chunk(NULL),
m_channel(-2)
{
	m_chunk = Mix_LoadWAV(m_soundName.c_str());
	if(!m_chunk){
		// Could not load file from disk
		Util::log(__FUNCTION__, 2, "Cannot load %s.", m_soundName.c_str());
	}
	else
	{
		Util::log(__FUNCTION__, 3, "%s loaded.", m_soundName.c_str());
	}
}


WavSample::~WavSample()
{
	if (m_chunk)
	{
		if (getState() != E_STOP)
		{
			Mix_HaltChannel(m_channel);
			m_state = E_STOP;
			Util::log(__FUNCTION__, 3, "%s halted.", m_soundName.c_str());
		}
		Mix_FreeChunk(m_chunk);
		Util::log(__FUNCTION__, 3, "%s released.", m_soundName.c_str());
	}
}


void
WavSample::play(bool loop)
{
	if (getState() == E_STOP)
	{
		if (loop)
		{
			m_channel = Mix_PlayChannel(-1, m_chunk, -1);
		}
		else
		{
			m_channel = Mix_PlayChannel(-1, m_chunk, 1);
		}

		if (m_channel == -1)
		{
			Util::log(__FUNCTION__, 3, "Channel full cannot play %s.", m_soundName.c_str());
		}
		else
		{
			m_state = E_PLAYING;
			Util::log(__FUNCTION__, 4, "%s started.", m_soundName.c_str());
		}
	}
	else if (getState() == E_PAUSE)
	{
		Mix_Resume(m_channel);
		m_state = E_PLAYING;
	}
}


void
WavSample::stop()
{
	if (getState() == E_PLAYING)
	{
		Mix_HaltChannel(m_channel);
		m_state = E_STOP;
		Util::log(__FUNCTION__, 4, "%s stopped.", m_soundName.c_str());
	}
}


void
WavSample::resume()
{
	if (getState() == E_PAUSE)
	{
		Mix_Resume(m_channel);
		m_state = E_PLAYING;
		Util::log(__FUNCTION__, 4, "%s resumed.", m_soundName.c_str());
	}
}


void 
WavSample::pause()
{
	if (getState() == E_PLAYING)
	{
		Mix_Pause(m_channel);
		m_state = E_PAUSE;
		Util::log(__FUNCTION__, 4, "%s paused.", m_soundName.c_str());
	}
}


void
WavSample::mute()
{
	m_volume  = Mix_Volume(m_channel, -1);
	Mix_Volume(m_channel, 0);
}


void
WavSample::unmute()
{
	Mix_Volume(m_channel, m_volume);
}