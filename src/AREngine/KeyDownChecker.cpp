#include "arengine/KeyDownChecker.h"
#include "arengine/SmartSingleton.h"
#include "arengine/Util.h"

#include <sstream>

using namespace std;
using namespace arengine;


KeyDownChecker::KeyDownChecker(DataNode *checkerNode)
{
	if (checkerNode->getNodeName().compare("KeyDown") == 0)
	{
		if (!checkerNode->getAttributeAsString("keyCode").empty())
		{
			m_key = checkerNode->getAttributeAsInt("keyCode");
		}
		else
		{
			string key = checkerNode->getAttributeAsString("key");
			m_key = (key.c_str())[0];
		}

		m_kbh = SmartSingleton<KeyboardHandler>::getInstance();

		stringstream sstr;
		sstr << "KeyDownChecker::KeyDownChecker() : key = " << m_key;
		Util::log(sstr.str().c_str(), 4);
	}
	else
	{
		Util::log("KeyDownChecker::KeyDownChecker() : Not a KeyDown tag", 2);
	}
}


KeyDownChecker::~KeyDownChecker()
{
}


bool
KeyDownChecker::conditionValid(osg::Node *node)
{
	if (m_kbh->isKeyDown(m_key))
	{
		stringstream sstr;
		sstr << "KeyDown detected for keycode = " << m_key;
		Util::log(sstr.str().c_str(), 5);
		return true;
	}
	else
	{
		return false;
	}
}
