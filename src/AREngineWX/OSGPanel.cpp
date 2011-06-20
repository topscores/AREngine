#include "wx/wx.h"

#include "arengine/SmartSingleton.h"
#include "arengine/KeyboardHandler.h"
#include "arenginewx/OSGPanel.h"

#include "arenginewx/OSGCanvas.h"
#include "arenginewx/GraphicsWindowWX.h"

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

using namespace arengine;
using namespace arenginewx;

BEGIN_EVENT_TABLE(OSGPanel, wxPanel)
EVT_IDLE(OSGPanel::OnIdle)
EVT_KEY_DOWN(OSGPanel::OnKeyDown)
EVT_KEY_UP(OSGPanel::OnKeyUp)
END_EVENT_TABLE()


/* My frame constructor */
OSGPanel::OSGPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos,
				   const wxSize& size, long style ) 
				   : wxPanel( parent, id, pos, size, style )
{
	// create osg canvas
	//    - initialize
	int width, height;
	width = size.GetWidth();
	height = size.GetHeight();

	int *attributes = new int[7];
	attributes[0] = int(WX_GL_DOUBLEBUFFER);
	attributes[1] = WX_GL_RGBA;
	attributes[2] = WX_GL_DEPTH_SIZE;
	attributes[3] = 8;
	attributes[4] = WX_GL_STENCIL_SIZE;
	attributes[5] = 8;
	attributes[6] = 0;

	_canvas = new OSGCanvas(this, wxID_ANY, wxDefaultPosition,
		wxSize(width, height), wxNO_BORDER, wxT("osgviewerWX"), attributes);

	GraphicsWindowWX* gw = new GraphicsWindowWX(_canvas);

	_canvas->SetGraphicsWindow(gw);

	_viewer = new osgViewer::Viewer;
	_viewer->getCamera()->setGraphicsContext(gw);
	_viewer->getCamera()->setViewport(0, 0, width, height);
	_viewer->addEventHandler(new osgViewer::StatsHandler);
	_viewer->addEventHandler(SmartSingleton<KeyboardHandler>::getInstance());
	_viewer->addEventHandler(new osgViewer::WindowSizeHandler);

	//_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
}


void 
OSGPanel::setViewer(osgViewer::Viewer *viewer)
{
	_viewer = viewer;
}


osgViewer::Viewer*
OSGPanel::getViewer()
{
	return _viewer.get();
}


void 
OSGPanel::OnIdle(wxIdleEvent &event)
{
	_viewer->frame();

	event.RequestMore();
}


void
OSGPanel::OnKeyDown(wxKeyEvent &event)
{
	_canvas->SetFocus();
	_canvas->GetEventHandler()->ProcessEvent(event);
}


void
OSGPanel::OnKeyUp(wxKeyEvent &event)
{
	_canvas->SetFocus();
	_canvas->GetEventHandler()->ProcessEvent(event);
}