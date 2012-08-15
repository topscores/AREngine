#include "arengine/MouseDownChecker.h"
#include "arengine/SmartSingleton.h"
#include "arengine/Util.h"

using namespace std;
using namespace arengine;


MouseDownChecker::MouseDownChecker(DataNode *checkerNode)
:ConditionChecker(checkerNode)
{
	if (checkerNode->getNodeName().compare("MouseDown") == 0)
	{
		if (!checkerNode->getAttributeAsString("objName").empty())
		{
			m_objName = checkerNode->getAttributeAsString("objName");
		}

		m_mh = SmartSingleton<MouseHandler>::getInstance();

		Util::log(__FUNCTION__, 3, "objName=%s", m_objName.c_str());
	}
	else
	{
		Util::log(__FUNCTION__, 2, "Not a MouseDown tag");
	}
}


MouseDownChecker::~MouseDownChecker()
{
}


bool
MouseDownChecker::conditionValid(osg::Node *node)
{
	if (m_mh->isMouseDown(m_objName))
	{
		Util::log(__FUNCTION__, 4, "MouseDown detected for objName = %s", m_objName.c_str());
		return true;
	}
	else
	{
		return false;
	}
}
