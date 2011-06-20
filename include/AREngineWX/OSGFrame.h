#ifndef __OSGART_FRAME_H__
#define __OSGART_FRAME_H__

#include "arenginewx/Export"

#include <osg/Node>
#include <osgViewer/Viewer>

#include <string>

using namespace osg;

namespace arenginewx
{
	class ARENGINEWX_EXPORT OSGFrame : public wxFrame
	{
	public:
		OSGFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
			const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

		void setViewer(osgViewer::Viewer *viewer);
		osgViewer::Viewer* getViewer();

		void OnIdle(wxIdleEvent& event);

	private:
		ref_ptr<osgViewer::Viewer> _viewer;

		DECLARE_EVENT_TABLE()
	};

}

#endif