#include "arengine/ObjPool.h"
#include "arengine/Singleton.h"
#include "arengine/Util.h"

using namespace arengine;

Singleton<ActionPool>::InstPtr Singleton<ActionPool>::sm_ptr;
Singleton<SceneObjPool>::InstPtr Singleton<SceneObjPool>::sm_ptr;