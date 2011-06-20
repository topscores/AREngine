#ifndef __LGSPLASHWX_H__
#define __LGSPLASHWX_H__

#include "arenginewx/Export"

#include "wx/wx.h"
#include "wx/event.h"
#include "wx/image.h"
#include "wx/splash.h"

#include <string>

using namespace std;

namespace arenginewx
{

	class ARENGINEWX_EXPORT LGSplashWx : public wxEvtHandler
	{
	public:
		LGSplashWx(wxString bitmapName);
		~LGSplashWx();

		void show();
		void show(int timeout);
		void close();

		void SetMainFrame(wxFrame *mainFrame, bool fullscreen);
		void OnSplashClose(wxCloseEvent& event);

	private:
		DECLARE_EVENT_TABLE()

	private:
		wxString		m_bitmapName;
		wxFrame			*m_mainFrame;
		bool			m_fullscreen;
		wxSplashScreen  *m_splash;
		wxBitmap		m_bitmap;
	};

}

#endif