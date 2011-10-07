#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "arenginewx/OSGCanvas.h"

using namespace arenginewx;


BEGIN_EVENT_TABLE(OSGCanvas, wxGLCanvas)
EVT_SIZE                (OSGCanvas::OnSize)
EVT_PAINT               (OSGCanvas::OnPaint)
EVT_ERASE_BACKGROUND    (OSGCanvas::OnEraseBackground)

EVT_CHAR                (OSGCanvas::OnChar)
EVT_KEY_UP              (OSGCanvas::OnKeyUp)
EVT_KEY_DOWN			(OSGCanvas::OnKeyDown)

EVT_ENTER_WINDOW        (OSGCanvas::OnMouseEnter)
EVT_LEFT_DOWN           (OSGCanvas::OnMouseDown)
EVT_MIDDLE_DOWN         (OSGCanvas::OnMouseDown)
EVT_RIGHT_DOWN          (OSGCanvas::OnMouseDown)
EVT_LEFT_UP             (OSGCanvas::OnMouseUp)
EVT_MIDDLE_UP           (OSGCanvas::OnMouseUp)
EVT_RIGHT_UP            (OSGCanvas::OnMouseUp)
EVT_MOTION              (OSGCanvas::OnMouseMotion)
END_EVENT_TABLE()


OSGCanvas::OSGCanvas(wxWindow *parent, wxWindowID id,
					 const wxPoint& pos, const wxSize& size, long style, const wxString& name, int *attributes)
					 : wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name, attributes)
{
	// default cursor to standard
	_oldCursor = *wxSTANDARD_CURSOR;
	_process_kd_evt = false;
}


OSGCanvas::~OSGCanvas()
{
}


void
OSGCanvas::enableProcessKeyDownEvent(bool processEvt)
{
	_process_kd_evt = processEvt;
}


void 
OSGCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
	/* must always be here */
	wxPaintDC dc(this);
}


void 
OSGCanvas::OnSize(wxSizeEvent& event)
{
	// this is also necessary to update the context on some platforms
	wxGLCanvas::OnSize(event);

	// set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
	int width, height;
	GetClientSize(&width, &height);

	if (_graphics_window.valid())
	{
		// update the window dimensions, in case the window has been resized.
		_graphics_window->getEventQueue()->windowResize(0, 0, width, height);
		_graphics_window->resized(0,0,width,height);
	}
}


void 
OSGCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
	/* Do nothing, to avoid flashing on MSW */
}


void 
OSGCanvas::OnChar(wxKeyEvent &event)
{
#if wxUSE_UNICODE
	int key = event.GetUnicodeKey();
#else
	int key = event.GetKeyCode();
#endif

	if (_graphics_window.valid())
		_graphics_window->getEventQueue()->keyPress(key);

	// If this key event is not processed here, we should call
	// event.Skip() to allow processing to continue.
}


void 
OSGCanvas::OnKeyUp(wxKeyEvent &event)
{
#if wxUSE_UNICODE
	int key = event.GetUnicodeKey();
#else
	int key = event.GetKeyCode();
#endif

	if (_graphics_window.valid())
		_graphics_window->getEventQueue()->keyRelease(key);

	// If this key event is not processed here, we should call
	// event.Skip() to allow processing to continue.
}


void 
OSGCanvas::OnMouseEnter(wxMouseEvent &event)
{
	// Set focus to ourselves, so keyboard events get directed to us
	//SetFocus();
}


void 
OSGCanvas::OnMouseDown(wxMouseEvent &event)
{
	if (_graphics_window.valid())
	{
		_graphics_window->getEventQueue()->mouseButtonPress(event.GetX(), event.GetY(),
			event.GetButton());
	}
}


void 
OSGCanvas::OnMouseUp(wxMouseEvent &event)
{
	if (_graphics_window.valid())
	{
		_graphics_window->getEventQueue()->mouseButtonRelease(event.GetX(), event.GetY(),
			event.GetButton());
	}
}


void 
OSGCanvas::OnMouseMotion(wxMouseEvent &event)
{
	if (_graphics_window.valid())
		_graphics_window->getEventQueue()->mouseMotion(event.GetX(), event.GetY());
}


void 
OSGCanvas::UseCursor(bool value)
{
	if (value)
	{
		// show the old cursor
		SetCursor(_oldCursor);
	}
	else
	{
		// remember the old cursor
		_oldCursor = GetCursor();

		// hide the cursor
		//    - can't find a way to do this neatly, so create a 1x1, transparent image
		wxImage image(1,1);
		image.SetMask(true);
		image.SetMaskColour(0, 0, 0);
		wxCursor cursor(image);
		SetCursor(cursor);

		// On wxGTK, only works as of version 2.7.0
		// (http://trac.wxwidgets.org/ticket/2946)
		// SetCursor( wxStockCursor( wxCURSOR_BLANK ) );
	}
}


void
OSGCanvas::OnKeyDown(wxKeyEvent &event)
{
#if wxUSE_UNICODE
	int key = event.GetUnicodeKey();
#else
	int key = event.GetKeyCode();
#endif
	if (_graphics_window.valid())
		_graphics_window->getEventQueue()->keyPress(key);

	if (_process_kd_evt)
	{
		switch (key)
		{
		case WXK_ESCAPE:
			GetParent()->Close();
			break;
			// Toggle fullscreen mode
		case 'f':
		case 'F':
			wxFrame *frame = dynamic_cast<wxFrame*>(GetParent());
			bool fullscreen = frame->IsFullScreen();
			frame->ShowFullScreen(!fullscreen, wxFULLSCREEN_ALL);
			frame->Show();
			break;
		}
	}
	else
	{
		switch (key)
		{
		case WXK_ESCAPE:
		case 'f':
		case 'F':
			GetParent()->GetParent()->GetEventHandler()->ProcessEvent(event);
			break;
		}
	}
}