#ifndef __OSGART_PANEL_H__
#define __OSGART_PANEL_H__

#include "arenginewx/Export"
#include "arenginewx/OSGCanvas.h"

#include <osg/Node>
#include <osgViewer/Viewer>

#include <string>

using namespace osg;

namespace arenginewx
{
	class ARENGINEWX_EXPORT OSGPanel : public wxPanel
	{
	public:
		OSGPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxSize( 800, 600), long style = wxTAB_TRAVERSAL|wxDOUBLE_BORDER);

		OSGCanvas *getCanvas(){return _canvas;}
		void setViewer(osgViewer::Viewer *viewer);
		osgViewer::Viewer* getViewer();

		void OnIdle(wxIdleEvent& event);
		void OnKeyDown(wxKeyEvent& event);
		void OnKeyUp(wxKeyEvent& event);

	private:
		ref_ptr<osgViewer::Viewer> _viewer;
		OSGCanvas *_canvas;

		DECLARE_EVENT_TABLE()
	};

}

#endif