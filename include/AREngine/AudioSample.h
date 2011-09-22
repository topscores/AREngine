#ifndef __AUDIO_SAMPLE_H_
#define __AUDIO_SAMPLE_H_

#include "arengine/Export"

#include "SDL.h"
#include "SDL_mixer.h"

#include <string>
using namespace std;

namespace arengine
{
	enum ESampleState
	{
		E_PLAYING,
		E_STOP,
		E_PAUSE
	};

	class ARENGINE_EXPORT Sample
	{
	public:
		Sample(string soundName)
		{
			m_soundName = soundName;
			m_state = E_STOP;
		}

		virtual void play(bool loop) = 0;
		virtual void stop() = 0;

		virtual void resume() = 0;
		virtual void pause() = 0;

		virtual ESampleState getState(){return m_state;}

	protected:
		string	m_soundName;
		ESampleState m_state;
	};

	class ARENGINE_EXPORT WavSample : public Sample
	{
	public:
		WavSample(string soundName);
		~WavSample();

		// Override method
		virtual void play(bool loop);
		virtual void stop();

		virtual void resume();
		virtual void pause();

		// virtual ESampleState getState();

	protected:
		Mix_Chunk *m_chunk;
		int m_channel;
	};

}

#endif