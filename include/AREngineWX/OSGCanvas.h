#ifndef __OSGCANVAS_H__
#define __OSGCANVAS_H__

#include "arenginewx/Export"

#include "wx/defs.h"
#include "wx/glcanvas.h"
#include "wx/cursor.h"

#include <osgViewer/Viewer>

using namespace osg;

namespace arenginewx
{

	class ARENGINEWX_EXPORT OSGCanvas : public wxGLCanvas
	{
	public:
		OSGCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize, long style = 0,
			const wxString& name = wxT("TestGLCanvas"),
			int *attributes = 0);

		virtual ~OSGCanvas();

		void SetGraphicsWindow(osgViewer::GraphicsWindow *gw)   { _graphics_window = gw; }
		osgViewer::GraphicsWindow *GetGraphicsWindow(){return _graphics_window;}
		void enableProcessKeyDownEvent(bool processEvt);

		void OnPaint(wxPaintEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnEraseBackground(wxEraseEvent& event);

		void OnChar(wxKeyEvent &event);
		void OnKeyUp(wxKeyEvent &event);
		void OnKeyDown(wxKeyEvent &event);

		void OnMouseEnter(wxMouseEvent &event);
		void OnMouseDown(wxMouseEvent &event);
		void OnMouseUp(wxMouseEvent &event);
		void OnMouseMotion(wxMouseEvent &event);

		void UseCursor(bool value);

	private:
		DECLARE_EVENT_TABLE()

		bool _process_kd_evt;
		ref_ptr<osgViewer::GraphicsWindow> _graphics_window;

		wxCursor _oldCursor;
	};

}

#endif