/*#include "arengine/Singleton.h"

#include "arengine/Logger.h"
#include "arengine/Config.h"
#include "arengine/ObjPool.h"
#include "arengine/SDLSoundManager.h"

#define IN_

using namespace arengine;

#ifdef WIN32
	template class ARENGINE_EXPORT Singleton<Logger>;
	template<>
	ARENGINE_EXPORT Singleton<Logger>::InstPtr Singleton<Logger>::sm_ptr;

	template class ARENGINE_EXPORT Singleton<Config>;
	template<>
	ARENGINE_EXPORT Singleton<Config>::InstPtr Singleton<Config>::sm_ptr;

	template class ARENGINE_EXPORT Singleton<ActionPool>;
	template<>
	ARENGINE_EXPORT Singleton<ActionPool>::InstPtr Singleton<ActionPool>::sm_ptr;

	template class ARENGINE_EXPORT Singleton<SceneObjPool>;
	template<>
	ARENGINE_EXPORT Singleton<SceneObjPool>::InstPtr Singleton<SceneObjPool>::sm_ptr;

	template class ARENGINE_EXPORT Singleton<SDLSoundManager>;
	template<>
	ARENGINE_EXPORT Singleton<SDLSoundManager>::InstPtr Singleton<SDLSoundManager>::sm_ptr;
#endif

#ifdef __APPLE__
template class ARENGINE_EXPORT Singleton<Logger>;
Singleton<Logger>::InstPtr Singleton<Logger>::sm_ptr;

template class ARENGINE_EXPORT Singleton<Config>;
Singleton<Config>::InstPtr Singleton<Config>::sm_ptr;

template class ARENGINE_EXPORT Singleton<ActionPool>;
Singleton<ActionPool>::InstPtr Singleton<ActionPool>::sm_ptr;

template class ARENGINE_EXPORT Singleton<SceneObjPool>;
Singleton<SceneObjPool>::InstPtr Singleton<SceneObjPool>::sm_ptr;

template class ARENGINE_EXPORT Singleton<SDLSoundManager>;
Singleton<SDLSoundManager>::InstPtr Singleton<SDLSoundManager>::sm_ptr;
#endif*/