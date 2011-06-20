#include "wx/wx.h"

#include "arenginewx/GraphicsWindowWX.h"

using namespace arenginewx;


GraphicsWindowWX::GraphicsWindowWX(OSGCanvas *canvas)
{
    _canvas = canvas;

    _traits = new GraphicsContext::Traits;

    wxPoint pos = _canvas->GetPosition();
    wxSize  size = _canvas->GetSize();

    _traits->x = pos.x;
    _traits->y = pos.y;
    _traits->width = size.x;
    _traits->height = size.y;

    init();
}


GraphicsWindowWX::~GraphicsWindowWX()
{
}


void 
GraphicsWindowWX::init()
{
    if (valid())
    {
        setState( new osg::State );
        getState()->setGraphicsContext(this);

        if (_traits.valid() && _traits->sharedContext)
        {
            getState()->setContextID( _traits->sharedContext->getState()->getContextID() );
            incrementContextIDUsageCount( getState()->getContextID() );
        }
        else
        {
            getState()->setContextID( osg::GraphicsContext::createNewContextID() );
        }
    }
}


void 
GraphicsWindowWX::grabFocus()
{
    // focus the canvas
    _canvas->SetFocus();
}


void 
GraphicsWindowWX::grabFocusIfPointerInWindow()
{
    // focus this window, if the pointer is in the window
    wxPoint pos = wxGetMousePosition();
    if (wxFindWindowAtPoint(pos) == _canvas)
        _canvas->SetFocus();
}


void 
GraphicsWindowWX::useCursor(bool cursorOn)
{
    _canvas->UseCursor(cursorOn);
}


bool 
GraphicsWindowWX::makeCurrentImplementation()
{
    _canvas->SetCurrent();
    return true;
}


void 
GraphicsWindowWX::swapBuffersImplementation()
{
    _canvas->SwapBuffers();
}