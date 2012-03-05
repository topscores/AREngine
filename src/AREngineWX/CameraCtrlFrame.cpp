#include "arenginewx/CameraCtrlFrame.h"
#include "arengine/AREngine.h"
#include "arengine/ARScene.h"
#include "arengine/Util.h"
#include "arengine/CaptureDeviceManager.h"

using namespace arengine;
using namespace arenginewx;

#define wxID_TOGGLEFULLSCREEN 1000

#ifdef WIN32
#	define MAX_DEV 10
#	define wxID_DEVFIRST 100
#	define wxID_DEVLAST (wxID_DEVFIRST + MAX_DEV - 1)
#	define wxID_SHOWPINPROPERTIES (wxID_DEVLAST + 1)
#	define wxID_SHOWFILTERPROPERTIES (wxID_SHOWPINPROPERTIES + 1)

BEGIN_EVENT_TABLE (CameraCtrlFrame, OSGFrame)
	EVT_MENU(wxID_SHOWPINPROPERTIES, CameraCtrlFrame::OnShowPin)
	EVT_MENU(wxID_SHOWFILTERPROPERTIES, CameraCtrlFrame::OnShowFilter)
	EVT_MENU_RANGE(wxID_DEVFIRST, wxID_DEVLAST, CameraCtrlFrame::OnSwitchDevices)
	EVT_MENU(wxID_TOGGLEFULLSCREEN, CameraCtrlFrame::OnToggleFullScreen)
	EVT_MENU(wxID_EXIT, CameraCtrlFrame::OnExit)
	EVT_CONTEXT_MENU(CameraCtrlFrame::OnContextMenu)
END_EVENT_TABLE ()

#endif

#ifdef __APPLE__
#	define wxID_DEVCONF 100

BEGIN_EVENT_TABLE (CameraCtrlFrame, OSGFrame)
	EVT_MENU(wxID_DEVCONF, CameraCtrlFrame::OnDeviceConfig)
	EVT_MENU(wxID_TOGGLEFULLSCREEN, CameraCtrlFrame::OnToggleFullScreen)
	EVT_MENU(wxID_EXIT, CameraCtrlFrame::OnExit)
	EVT_CONTEXT_MENU(CameraCtrlFrame::OnContextMenu)
END_EVENT_TABLE ()
#endif


CameraCtrlFrame::CameraCtrlFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
			const wxSize& size, long style)
:OSGFrame(frame, title, pos, size, style)
{
	m_menubar = new wxMenuBar();
	SetMenuBar(m_menubar);

#ifdef WIN32
	// Create menubar
	wxMenu *learngearMenu  = new wxMenu();
	learngearMenu->Append(wxID_TOGGLEFULLSCREEN, wxT("Fullscreen"));
	learngearMenu->AppendSeparator();
	learngearMenu->Append(wxID_EXIT, wxT("Exit"));
	
	m_menubar->Append(learngearMenu, wxT("Learngear"));
	m_menubar->Append(createDevMenu(), wxT("Devices"));
	m_menubar->Append(createOptionMenu(), wxT("Options"));

	// Create Context Menu
	m_contextMenu.AppendSubMenu(createDevMenu(), wxT("Devices"));
	m_contextMenu.AppendSubMenu(createOptionMenu(), wxT("Option"));
	m_contextMenu.Append(wxID_TOGGLEFULLSCREEN, wxT("Toggle FullScreen"));
	m_contextMenu.AppendSeparator();
	m_contextMenu.Append(wxID_EXIT, wxT("Exit"));
#endif
	
#ifdef __APPLE__
	// Create menubar
	wxMenu *learngearMenu  = new wxMenu();
	learngearMenu->Append(wxID_DEVCONF, wxT("Config Capture Device"));
	learngearMenu->Append(wxID_TOGGLEFULLSCREEN, wxT("Fullscreen"));
	
	m_menubar->Append(learngearMenu, wxT("Learngear"));


	// Create Context Menu
	m_contextMenu.Append(wxID_DEVCONF, wxT("Config Capture Device"));
	m_contextMenu.Append(wxID_TOGGLEFULLSCREEN, wxT("Toggle FullScreen"));
#endif
}


CameraCtrlFrame::~CameraCtrlFrame()
{
}


bool
CameraCtrlFrame::WaitForCaptureDevice()
{
	while(!AREngine::isCaptureDeviceReady())
	{
		wxMessageDialog dlg(NULL, 
			wxT("Could not find available capture device"), 
			wxT("Fatal Error"), 
			wxOK|wxCANCEL);

		if (wxID_CANCEL == dlg.ShowModal())
		{
			return false;
		}
	}
	return true;
}

void
CameraCtrlFrame::OnContextMenu(wxContextMenuEvent &event)
{
	PopupMenu(&m_contextMenu);
}


void
CameraCtrlFrame::OnToggleFullScreen(wxCommandEvent &event)
{
	bool fullscreen = IsFullScreen();
	ShowFullScreen(!fullscreen, wxFULLSCREEN_ALL);
	Show();
}


void
CameraCtrlFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}


#ifdef WIN32

void
CameraCtrlFrame::OnSwitchDevices(wxCommandEvent &event)
{
	int devid = event.GetId();
	ARScene *arscene = AREngine::getARScene();
	if (arscene)
	{
		arscene->changeCaptureDevice(devid - wxID_DEVFIRST);
	}
}


void
CameraCtrlFrame::OnShowPin(wxCommandEvent &event)
{
	ARScene *arscene = AREngine::getARScene();
	if (arscene)
	{
		arscene->showPinProperties((HWND)this->GetHandle());
	}
}


void
CameraCtrlFrame::OnShowFilter(wxCommandEvent &event)
{
	ARScene *arscene = AREngine::getARScene();
	if (arscene)
	{
		arscene->showFilterProperties((HWND)this->GetHandle());
	}
}


// Private method
wxMenu*
CameraCtrlFrame::createDevMenu()
{
	int n = CaptureDeviceManager::getDeviceCount();
	wxMenu *menu = new wxMenu();
	string displayName;
	int frame_width, frame_height;
	CaptureDeviceManager::loadCapDevInfo(displayName, frame_width, frame_height);
	for (int i = 0;i < n;i++)
	{
        menu->AppendRadioItem(wxID_DEVFIRST + i, wxString(Util::widen(CaptureDeviceManager::getFriendlyName(i))));
		if (displayName == CaptureDeviceManager::getDisplayName(i))
		{
			menu->Check(wxID_DEVFIRST + i, true);
		}
	}

	return menu;
}


wxMenu*
CameraCtrlFrame::createOptionMenu()
{
	wxMenu *menu = new wxMenu();
	menu->Append(wxID_SHOWPINPROPERTIES, wxT("Change Video Resolution"));
	menu->Append(wxID_SHOWFILTERPROPERTIES, wxT("Adjust Video Properties"));

	return menu;
}

#endif

#ifdef __APPLE__

void
CameraCtrlFrame::OnDeviceConfig(wxCommandEvent &event)
{
	ARScene *arscene = AREngine::getARScene();
	if (arscene)
	{
		arscene->showDeviceConfig();
	}
}


#endif
