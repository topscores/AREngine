#include "wx/wx.h"

#include "arengine/KeyboardHandler.h"
#include "arengine/SmartSingleton.h"

#include "arenginewx/OSGFrame.h"
#include "arenginewx/OSGCanvas.h"
#include "arenginewx/GraphicsWindowWX.h"

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <string>

using namespace arengine;
using namespace arenginewx;


BEGIN_EVENT_TABLE(OSGFrame, wxFrame)
    EVT_IDLE(OSGFrame::OnIdle)
END_EVENT_TABLE()


/* My frame constructor */
OSGFrame::OSGFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style)
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

    OSGCanvas *canvas = new OSGCanvas(this, wxID_ANY, wxDefaultPosition,
		wxSize(width, height), wxNO_BORDER, wxT("osgviewerWX"), attributes);
	canvas->enableProcessKeyDownEvent(true);

    GraphicsWindowWX* gw = new GraphicsWindowWX(canvas);

    canvas->SetGraphicsWindow(gw);

	_viewer = new osgViewer::Viewer;
    _viewer->getCamera()->setGraphicsContext(gw);
	_viewer->getCamera()->setViewport(0, 0, width, height);
    _viewer->addEventHandler(new osgViewer::StatsHandler);
	_viewer->addEventHandler(SmartSingleton<KeyboardHandler>::getInstance());
	_viewer->addEventHandler(new osgViewer::WindowSizeHandler);
    //_viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
}


void 
OSGFrame::setViewer(osgViewer::Viewer *viewer)
{
    _viewer = viewer;
}


osgViewer::Viewer*
OSGFrame::getViewer()
{
	return _viewer.get();
}


void 
OSGFrame::OnIdle(wxIdleEvent &event)
{
    _viewer->frame();

    event.RequestMore();
}
