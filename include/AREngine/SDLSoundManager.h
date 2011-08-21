#ifndef __SDL_SOUND_MANAGER_H__
#define __SDL_SOUND_MANAGER_H__

#include "arengine/Export"
#include "arengine/AudioSample.h"

#include <string>
#include <map>
using namespace std;

// Audio CD sample rate so most likely all hardware will support
#define SAMPLE_RATE 44100
// Stereo
#define PLAYBACK_CHANNELS 2 
#define CONCURRENT_SOUNDS 8

namespace arengine
{

	class ARENGINE_EXPORT SDLSoundManager
	{
	public:
		void initAudio();
		void closeAudio();

		void play(string soundName, bool loop);
		void stop(string soundName="");
		void pause(string soundName="");

	private:
		Sample *getSample(string soundName);

	private:
		map<string, Sample*> m_samples;
	};

}

#endif