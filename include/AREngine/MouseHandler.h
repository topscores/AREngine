#ifndef __MOUSE_HANDLER_H__
#define __MOUSE_HANDLER_H__

#include "arengine/Export"
#include "arengine/SmartSingleton.h"
#include "arengine/Util.h"

#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

namespace arengine
{

	// This class should be access via Singleton class
	class ARENGINE_EXPORT MouseHandler : public osgGA::GUIEventHandler
	{ 
		template<class MouseHandler> friend class SmartSingleton;
	public:


		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
		bool isMouseDown(string objname);

	private:
		MouseHandler();
		MouseHandler(const KeyboardHandler&);
		MouseHandler& operator=(const KeyboardHandler&);

		void check2DIntersection(int mx, int my);
		void check3DIntersection(int mx, int my);
		void setMouseDown(string objName);

	private:
		std::map <string, bool> m_isPick;

	};

}

#endif