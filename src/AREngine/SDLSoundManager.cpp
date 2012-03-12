#include "SDL.h"
#include "SDL_mixer.h"

#include "arengine/SDLSoundManager.h"
#include "arengine/Util.h"
#include "arengine/AudioSample.h"

using namespace arengine;


void
SDLSoundManager::initAudio()
{
	if(SDL_Init(SDL_INIT_AUDIO) == -1)
	{
		Util::log(__FUNCTION__, 2, "Could not initialize SDL_Audio.");
	}
	else
	{
		Util::log(__FUNCTION__, 3, "SDL_Audio initialized.");
	}

	if (Mix_OpenAudio(SAMPLE_RATE, MIX_DEFAULT_FORMAT, PLAYBACK_CHANNELS, 1024) == -1)
	{
		Util::log(__FUNCTION__, 2, "Could not initialize SDL_Mixer");
	}
	else
	{
		Util::log(__FUNCTION__, 3,  "SDL_mixer initialized.");
	}

	Mix_AllocateChannels(CONCURRENT_SOUNDS);
}


void
SDLSoundManager::closeAudio()
{
	map<string, Sample*>::iterator itr;
	for (itr = m_samples.begin();itr != m_samples.end();itr++)
	{
		Sample *sample = (*itr).second;
		if (sample->getState() == E_PLAYING 
			|| sample->getState() == E_PAUSE)
		{
			sample->stop();
		}
		delete sample;
	}
	m_samples.clear();

	Mix_CloseAudio();
	Util::log(__FUNCTION__, 3, "Close SDL_Mixer");

	SDL_Quit();
	Util::log(__FUNCTION__, 3, "Close SDL_Audio");
}


void
SDLSoundManager::play(string soundName, bool loop)
{
	Sample *sample = getSample(soundName);

	// Sample of same sound file has already existed
	if (sample)
	{
		if (sample->getState() == E_STOP)
		{
			sample->play(loop);
		}
		else 
		{
			sample->resume();
		}
	}
	else
	{
		string soundType = soundName.substr(soundName.find_last_of(".") + 1);
		Sample *sample;
		if (soundType == "wav")
		{
			sample = new WavSample(soundName);
		}

		if (sample)
		{
			m_samples.insert(pair<string, Sample*>(soundName, sample));
			sample->play(loop);
		}
	}
}


void
SDLSoundManager::stop(string soundName)
{
	if (soundName.empty())
	{
		map<string, Sample*>::iterator itr;
		for (itr = m_samples.begin();itr != m_samples.end();itr++)
		{
			Sample *sample = (*itr).second;
			sample->stop();
		}
	}
	else
	{
		Sample *sample = getSample(soundName);
		if (sample)
		{
			sample->stop();
		}
	}
}


void
SDLSoundManager::pause(string soundName)
{
	if (soundName.empty())
	{
		map<string, Sample*>::iterator itr;
		for (itr = m_samples.begin();itr != m_samples.end();itr++)
		{
			Sample *sample = (*itr).second;
			sample->pause();
		}
	}
	else
	{
		Sample *sample = getSample(soundName);
		if (sample)
		{
			sample->pause();
		}
	}
}


Sample*
SDLSoundManager::getSample(string soundName)
{
	map<string, Sample*>::iterator itr;
	itr = m_samples.find(soundName);

	// Sample already existed
	if (itr != m_samples.end())
	{
		return (*itr).second;
	}
	// Not Found
	else
	{
		return NULL;
	}
}


void
SDLSoundManager::toggleMute()
{
	map<string, Sample*>::iterator itr;
	for (itr = m_samples.begin();itr != m_samples.end();itr++)
	{
		Sample *sample = itr->second;
		if (!m_mute)
		{
			sample->mute();
		}
		else
		{
			sample->unmute();
		}
	}
	m_mute = !m_mute;
}