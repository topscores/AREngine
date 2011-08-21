#include "arengine/SmartSingleton.h"
#include "arengine/ARScene.h"
#include "arengine/KeyboardHandler.h"

#include <osg/Node>

#define IN_

using namespace arengine;
using namespace osg;

template class ARENGINE_EXPORT SmartSingleton<ARScene>;
template<>
ARENGINE_EXPORT ref_ptr<ARScene> SmartSingleton<ARScene>::sm_ptr;

template class ARENGINE_EXPORT SmartSingleton<KeyboardHandler>;
template<>
ARENGINE_EXPORT ref_ptr<KeyboardHandler> SmartSingleton<KeyboardHandler>::sm_ptr;

