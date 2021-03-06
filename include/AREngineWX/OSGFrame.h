#ifndef __OSGART_FRAME_H__
#define __OSGART_FRAME_H__

#include "wx/wx.h"

#include "arenginewx/Export"
#include "arenginewx/OSGCanvas.h"

#include <osgViewer/Viewer>

#include <string>

namespace arenginewx
{
	class ARENGINEWX_EXPORT OSGFrame : public wxFrame
	{
	public:
		OSGFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
			const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

		OSGCanvas *getCanvas(){return _canvas;}
		void setViewer(osgViewer::Viewer *viewer);
		osgViewer::Viewer* getViewer();

	private:
		void OnIdle(wxIdleEvent& event);

	private:
		OSGCanvas *_canvas;
		osg::ref_ptr<osgViewer::Viewer> _viewer;

		DECLARE_EVENT_TABLE()
	};

}

#endif