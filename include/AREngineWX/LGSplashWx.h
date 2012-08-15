#ifndef __LGSPLASHWX_H__
#define __LGSPLASHWX_H__

#include "arenginewx/Export"

#include "wx/wx.h"
#include "wx/event.h"
#include "wx/timer.h"
//#include "wx/image.h"
//#include "wx/splash.h"

#include <string>

using namespace std;

namespace arenginewx
{

	class ARENGINEWX_EXPORT LGSplashWx : public wxFrame
	{
	public:
		LGSplashWx(wxString bitmapName);
		~LGSplashWx();

		void SetMainFrame(wxFrame *mainFrame, bool fullscreen);
		
		void OnSplashClose(wxCloseEvent& event);

	private:
		DECLARE_EVENT_TABLE()

	private:
		wxFrame *m_mainFrame;
		bool m_fullscreen;
		wxTimer *m_timer;
	};
}

#endif