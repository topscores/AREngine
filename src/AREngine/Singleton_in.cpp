#include "arengine/Singleton.h"

#include "arengine/Logger.h"
#include "arengine/Config.h"
#include "arengine/ObjPool.h"
#include "arengine/SDLSoundManager.h"

#define IN_

using namespace arengine;

template class ARENGINE_EXPORT Singleton<Logger>;
template<>
ARENGINE_EXPORT typename Singleton<Logger>::InstPtr Singleton<Logger>::sm_ptr;

template class ARENGINE_EXPORT Singleton<Config>;
template<>
ARENGINE_EXPORT typename Singleton<Config>::InstPtr Singleton<Config>::sm_ptr;

template class ARENGINE_EXPORT Singleton<ActionPool>;
template<>
ARENGINE_EXPORT typename Singleton<ActionPool>::InstPtr Singleton<ActionPool>::sm_ptr;

template class ARENGINE_EXPORT Singleton<SceneObjPool>;
template<>
ARENGINE_EXPORT typename Singleton<SceneObjPool>::InstPtr Singleton<SceneObjPool>::sm_ptr;

template class ARENGINE_EXPORT Singleton<SDLSoundManager>;
template<>
ARENGINE_EXPORT typename Singleton<SDLSoundManager>::InstPtr Singleton<SDLSoundManager>::sm_ptr;