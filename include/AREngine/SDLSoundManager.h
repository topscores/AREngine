#ifndef __SDL_SOUND_MANAGER_H__
#define __SDL_SOUND_MANAGER_H__

#include "arengine/Export"
#include "arengine/Singleton.h"
#include "arengine/AudioSample.h"

#include <string>
#include <map>

// Audio CD sample rate so most likely all hardware will support
#define SAMPLE_RATE 44100
// Stereo
#define PLAYBACK_CHANNELS 2 
#define CONCURRENT_SOUNDS 8

namespace arengine
{

	class ARENGINE_EXPORT SDLSoundManager 
	{
	template<class SDLSoundManager> friend class Singleton;
	public:
		void initAudio();
		void closeAudio();

		void play(string soundName, bool loop);
		void stop(string soundName="");
		void pause(string soundName="");

		void toggleMute();

	private:
		SDLSoundManager()
			:m_mute(false)
		{
		}

		SDLSoundManager(const SDLSoundManager&){}
		SDLSoundManager& operator=(const SDLSoundManager&){return *this;}

		Sample *getSample(string soundName);

	private:
		std::map<string, Sample*> m_samples;
		bool m_mute;
	};

}

#endif