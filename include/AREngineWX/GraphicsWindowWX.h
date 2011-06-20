#ifndef __GRAPHICSWINDOW_WX_H__
#define __GRAPHICSWINDOW_WX_H__

#include "arenginewx/Export"
#include "arenginewx/OSGCanvas.h"

#include <osgViewer/Viewer>

namespace arenginewx
{

	class ARENGINEWX_EXPORT GraphicsWindowWX : public osgViewer::GraphicsWindow
	{
	public:
		GraphicsWindowWX(OSGCanvas *canvas);
		~GraphicsWindowWX();

		void init();

		//
		// GraphicsWindow interface
		//
		void grabFocus();
		void grabFocusIfPointerInWindow();
		void useCursor(bool cursorOn);

		bool makeCurrentImplementation();
		void swapBuffersImplementation();

		// not implemented yet...just use dummy implementation to get working.
		virtual bool valid() const { return true; }
		virtual bool realizeImplementation() { return true; }
		virtual bool isRealizedImplementation() const  { return true; }
		virtual void closeImplementation() {}
		virtual bool releaseContextImplementation() { return true; }

	private:
		// XXX need to set _canvas to NULL when the canvas is deleted by
		// its parent. for this, need to add event handler in OSGCanvas
		OSGCanvas*  _canvas;
	};

}
#endif