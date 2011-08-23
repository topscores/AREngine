#ifndef __TEST_CALLBACK_H__
#define __TEST_CALLBACK_H__

#include "arengine/Export"
#include "arengine/Util.h"

#include <osg/Node>

#include <string>

using namespace std;

namespace arengine
{
	class ARENGINE_EXPORT TestCallback : public osg::NodeCallback
	{
	public:
		TestCallback(string logMsg):m_logMsg(logMsg)
		{
		}

		virtual void operator()(osg::Node *node, osg::NodeVisitor *nv)
		{
			Util::log(m_logMsg.c_str(), 3);
		}

	private:
		string m_logMsg;
	};
}

#endif