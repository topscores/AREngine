#include "arengine/Util.h"
#include "arenginewx/LGSplashWx.h"

#include "wx/animate.h"

using namespace arengine;
using namespace arenginewx;

BEGIN_EVENT_TABLE(LGSplashWx, wxFrame)
	EVT_CLOSE(LGSplashWx::OnSplashClose)
END_EVENT_TABLE()

LGSplashWx::LGSplashWx(wxString bitmapName)
:wxFrame(NULL,
		 wxID_ANY,
		 wxT(""),
		 wxDefaultPosition, 
		 wxSize(605, 317),
		 wxDEFAULT_FRAME_STYLE & ~(wxCAPTION | wxRESIZE_BORDER))
{
	Center();

	m_mainFrame = NULL;

	wxSizer *sz = new wxBoxSizer(wxVERTICAL);

	wxAnimationCtrl *animation = new wxAnimationCtrl(this, wxID_ANY);
	animation->LoadFile(bitmapName);
	animation->Play();
	sz->Add(animation);

	SetSizer(sz);
}


LGSplashWx::~LGSplashWx()
{
}


void
LGSplashWx::SetMainFrame(wxFrame *mainFrame, bool fullscreen)
{
	m_mainFrame = mainFrame;
	m_fullscreen = fullscreen;
}


void
LGSplashWx::OnSplashClose(wxCloseEvent& event)
{
	if (m_mainFrame == NULL)
	{
		event.Skip();
		return;
	}

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