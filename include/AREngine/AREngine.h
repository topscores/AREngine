#ifndef __ARENGINE_H__
#define __ARENGINE_H__

#include "arengine/Export"

#include "arengine/Logger.h"
#include "arengine/Config.h"
#include "arengine/ARScene.h"

namespace arengine
{
	
	class ARENGINE_EXPORT AREngine
	{
	public:
		static Logger* getLogger();
		static Config* getConfig();
		static ARScene* getARScene();
	};
	
}

#endif