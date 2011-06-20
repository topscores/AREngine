#ifndef __KEYBOARD_HANDLER_H__
#define __KEYBOARD_HANDLER_H__

#include "arengine/Export"
#include "arengine/Util.h"

#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

#include <sstream>
using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT KeyboardHandler : public osgGA::GUIEventHandler
	{ 
	public:
		KeyboardHandler();

		bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
		bool isKeyDown(int key);

	private:
		bool m_keypress[500];

	};

}

#endif