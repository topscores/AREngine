#include "arengine/Util.h"
#include "arenginewx/LGSplashWx.h"

using namespace arengine;
using namespace arenginewx;

BEGIN_EVENT_TABLE(LGSplashWx, wxEvtHandler)
	EVT_CLOSE(LGSplashWx::OnSplashClose)
END_EVENT_TABLE()

LGSplashWx::LGSplashWx(wxString bitmapName)
:m_mainFrame(NULL), m_fullscreen(false)
{
#ifdef WIN32
	m_bitmapName = bitmapName;
#endif 
	
#ifdef __APPLE__
	string nativePath = Util::getNativePath(bitmapName.mb_str().data());
	m_bitmapName = wxString(nativePath.c_str(), wxConvUTF8);
#endif
}


LGSplashWx::~LGSplashWx()
{
	delete m_splash;
}


void
LGSplashWx::show()
{
	wxInitAllImageHandlers();
	m_bitmap.LoadFile(m_bitmapName, wxBITMAP_TYPE_PNG);

	m_splash = new wxSplashScreen(m_bitmap,
		wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_NO_TIMEOUT,
		0, NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxSIMPLE_BORDER|wxSTAY_ON_TOP);
	//wxWindow *win = splash->GetSplashWindow();

	m_splash->SetEventHandler(this);

	wxYield();
}


void
LGSplashWx::show(int timeout)
{
	wxInitAllImageHandlers();
	m_bitmap.LoadFile(m_bitmapName, wxBITMAP_TYPE_PNG);

	m_splash = new wxSplashScreen(m_bitmap,
		wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
		timeout, NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxSIMPLE_BORDER|wxSTAY_ON_TOP);

	wxYield();
}


void
LGSplashWx::close()
{
	m_splash->Close();
}


void
LGSplashWx::SetMainFrame(wxFrame *mainFrame, bool fullscreen)
{
	m_mainFrame = mainFrame;
	m_fullscreen = fullscreen;
}


void
LGSplashWx::OnSplashClose(wxCloseEvent &event)
{
	m_splash->OnCloseWindow(event);
	if (m_mainFrame == NULL)
		return;

	if (m_fullscreen)
	{
		m_mainFrame->Show(true);
		m_mainFrame->ShowFullScreen(true);
	}
	else
	{
		m_mainFrame->Show(true);
		m_mainFrame->ShowFullScreen(false);
	}

	event.Skip();
}
