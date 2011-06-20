#ifndef __ACTION_CALLBACK_H__
#define __ACTION_CALLBACK_H__

#include "arengine/Export"

#include <osg/Node>

namespace arengine
{
	class ARENGINE_EXPORT ActionCallback : public osg::Referenced
	{
	public:
		virtual void exec(osg::Node *node) = 0;
	};

}

#endif