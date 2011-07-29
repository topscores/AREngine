#ifndef __ACTION_FACTORY_H__
#define __ACTION_FACTORY_H__

#include "arengine/Action.h"
#include "arengine/DataNode.h"

#include <osg/Node>
using namespace osg;

namespace arengine
{

	class ARENGINE_EXPORT ActionCallbackFactory
	{
	public:
		static ref_ptr<ActionCallback> newCallback(DataNode *callback)
		{
			if (callback == NULL)
				return NULL;

			string type = callback->getAttributeAsString("type");
			if (type == "ScreenCapture")
			{
				//return new ScreenCapture();
				return NULL;
			}
			else
			{
				return NULL;
			}


		}
	};

	class ARENGINE_EXPORT ActionFactory
	{
	public:
		static ref_ptr<Action> newAction(DataNode *action);
	};
}

#endif