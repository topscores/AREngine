#include "arengine/KeyboardHandler.h"
#include "arengine/SmartSingleton.h"

using namespace arengine;

SmartSingleton<KeyboardHandler>::InstPtr SmartSingleton<KeyboardHandler>::sm_ptr;

KeyboardHandler::KeyboardHandler()
{
	for (int i = 0;i < 500;i++)
	{
		m_keypress[i] = false;
	}
}


bool 
KeyboardHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>( &aa );
	if (!viewer)
	{
		return false;
	}

	switch (ea.getEventType())
	{
		int key;
	case osgGA::GUIEventAdapter::KEYDOWN:
		key = ea.getKey();
		if (key != -1)
		{
			// Convert to lower-case
			if (key >= 65 && key <= 90)
			{
				key+=32;
			}

			m_keypress[key] = true;

			stringstream sstr;
			sstr << "KeyDown with id = " << key;
			Util::log(sstr.str().c_str(), 4);

			return false;
		}
		else
		{
			Util::log("Invalid keyId", 2);
			return false;
		}

	case osgGA::GUIEventAdapter::KEYUP:
		key = ea.getKey();
		if (key != -1)
		{
			// Convert to lower-case
			if (key >= 65 && key <= 90)
			{
				key+=32;
			}

			m_keypress[key] = false;

			stringstream sstr;
			sstr << "KeyUP with id = " << key;
			Util::log(sstr.str().c_str(), 4);

			return false;
		}
		else
		{
			Util::log("Invalid keyId", 2);
			return false;
		}
	}
}


bool
KeyboardHandler::isKeyDown(int key)
{
	if (key >= 0 && key <= 500)
	{
		return m_keypress[key];
	}
	else
	{
		Util::log("Invalid keycode", 2);
		return false;
	}
}