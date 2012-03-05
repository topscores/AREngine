#ifndef __ARENGINE_H__
#define __ARENGINE_H__

#include "arengine/Export"

#include "arengine/Logger.h"
#include "arengine/Config.h"
#include "arengine/ARScene.h"
#include "arengine/KeyboardHandler.h"

#ifdef _WIN32
#	include <dshow.h>
#endif

namespace arengine
{
	
	class ARENGINE_EXPORT AREngine
	{
	public:
		static bool isCaptureDeviceReady();
		static void init(string appName,
						 string logFileName, 
						 int logLevel,
						 string configFileName);

		static Logger* getLogger();
		static Config* getConfig();
		static ARScene* getARScene();
		static KeyboardHandler* getKeyboardHandler();
		
		static void release();
		static bool ready();
	private:
		static bool m_ready;

	};
	
}

#endif