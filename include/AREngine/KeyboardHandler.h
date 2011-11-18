#ifndef __KEYBOARD_HANDLER_H__
#define __KEYBOARD_HANDLER_H__

#include "arengine/Export"
#include "arengine/SmartSingleton.h"
#include "arengine/Util.h"

#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

#include <sstream>
#include <map>

namespace arengine
{

	// This class should be access via Singleton class
	class ARENGINE_EXPORT KeyboardHandler : public osgGA::GUIEventHandler
	{ 
	template<class KeyboardHandler> friend class SmartSingleton;
	public:


		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
		bool isKeyDown(int key);

	private:
		KeyboardHandler();
		KeyboardHandler(const KeyboardHandler&);
		KeyboardHandler& operator=(const KeyboardHandler&);

	private:
		std::map<int, bool> m_keypress;

	};

}

#endif