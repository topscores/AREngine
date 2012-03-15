#include "arengine/MouseOverChecker.h"
#include "arengine/SmartSingleton.h"
#include "arengine/Util.h"

using namespace std;
using namespace arengine;


MouseOverChecker::MouseOverChecker(DataNode *checkerNode)
:ConditionChecker(checkerNode)
{
	if (checkerNode->getNodeName().compare("MouseOver") == 0)
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
		Util::log(__FUNCTION__, 2, "Not a MouseOver tag");
	}
}


MouseOverChecker::~MouseOverChecker()
{
}


bool
MouseOverChecker::conditionValid(osg::Node *node)
{
	if (m_mh->isMouseOver(m_objName))
	{
		Util::log(__FUNCTION__, 4, "MouseOver detected for objName = %s", m_objName.c_str());
		return true;
	}
	else
	{
		return false;
	}
}
