#include "arenginewx/CameraCtrlFrame.h"
#include "arengine/AREngine.h"
#include "arengine/ARScene.h"
#include "arengine/Util.h"

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
#endif


CameraCtrlFrame::CameraCtrlFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
			const wxSize& size, long style)
:OSGFrame(frame, title, pos, size, style)
{
	m_menubar = new wxMenuBar();
	SetMenuBar(m_menubar);

	wxMenu *learngearMenu  = new wxMenu();
	learngearMenu->Append(wxID_TOGGLEFULLSCREEN, wxT("Fullscreen"));
	learngearMenu->AppendSeparator();
	learngearMenu->Append(wxID_EXIT, wxT("Exit"));
	m_menubar->Append(learngearMenu, wxT("Learngear"));


#ifdef WIN32
	// Create menubar
	m_menubar->Append(createDevMenu(), wxT("Devices"));
	m_menubar->Append(createOptionMenu(), wxT("Options"));

	SetMenuBar(m_menubar);

	// Create Context Menu
	m_contextMenu.AppendSubMenu(createDevMenu(), wxT("Devices"));
	m_contextMenu.AppendSubMenu(createOptionMenu(), wxT("Option"));
	m_contextMenu.Append(wxID_TOGGLEFULLSCREEN, wxT("Toggle FullScreen"));
	m_contextMenu.AppendSeparator();
	m_contextMenu.Append(wxID_EXIT, wxT("Exit"));
#endif
	
#ifdef __APPLE__
	wxMenu *devmenu = new wxMenu();
	devmenu->Append(wxID_DEVCONF, wxT("Configure"));
	m_menubar->Append(devmenu, wxT("Devices"));
	
	Connect(wxID_DEVCONF, 
			wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(CameraCtrlFrame::OnDeviceConfig));
#endif
}


CameraCtrlFrame::~CameraCtrlFrame()
{
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
		arscene->changeCaptureDevice(m_devls[devid - wxID_DEVFIRST].pSrcFilter);
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
	m_devls = Util::getDeviceList(CLSID_VideoInputDeviceCategory);
	wxMenu *menu = new wxMenu();
	for (int i = 0;i < m_devls.size();i++)
	{
		menu->AppendRadioItem(wxID_DEVFIRST + i, wxString(Util::widen(m_devls[i].friendlyName.c_str())));
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
