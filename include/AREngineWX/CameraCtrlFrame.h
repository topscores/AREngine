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
		
		// Return false if no capture devices were found
		bool WaitForCaptureDevice();
		bool ContinueWithoutLog();

		void OnFaq(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnContextMenu(wxContextMenuEvent& event);
		void OnToggleFullScreen(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);

#ifdef WIN32
		void OnSwitchDevices(wxCommandEvent& event);
		void OnShowPin(wxCommandEvent& event);
		void OnShowFilter(wxCommandEvent& event);
#endif
		
#ifdef __APPLE__
		void OnDeviceConfig(wxCommandEvent& event);
#endif

#ifdef WIN32
	private:
		wxMenu *createDevMenu();
		wxMenu *createOptionMenu();
		wxMenu *createAboutMenu();
#endif

		
	private:
		wxMenuBar *m_menubar;
		wxMenu m_contextMenu;
		int m_devcount;

		DECLARE_EVENT_TABLE();
	};
}

#endif