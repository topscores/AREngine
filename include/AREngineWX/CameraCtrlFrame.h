#ifndef __CAMERA_CTRL_FRAME_H__
#define __CAMERA_CTRL_FRAME_H__

#include "wx/menu.h"

#include "arenginewx/Export"
#include "arenginewx/OSGFrame.h"
#include "arengine/Util.h"

#include <vector>
using namespace std;

namespace arenginewx
{
	class ARENGINEWX_EXPORT CameraCtrlFrame : public OSGFrame
	{
	public:
		CameraCtrlFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
			const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
		~CameraCtrlFrame();

#ifdef _WIN32
		void OnSwitchDevices(wxCommandEvent& event);
		void OnShowPin(wxCommandEvent& event);
		void OnShowFilter(wxCommandEvent& event);
#endif

	private:
		wxMenuBar *m_menubar;
		int m_devcount;
		vector<DEVINFO> m_devls;
	};
}

#endif