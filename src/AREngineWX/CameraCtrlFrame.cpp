#include "arenginewx/CameraCtrlFrame.h"
#include "arengine/AREngine.h"
#include "arengine/ARScene.h"
#include "arengine/Util.h"

using namespace arengine;
using namespace arenginewx;

#ifdef WIN32
#	define MAX_DEV 10
#	define wxID_DEVFIRST 100
#	define wxID_DEVLAST (wxID_DEVFIRST + MAX_DEV - 1)
#	define wxID_SHOWPINPROPERTIES (wxID_DEVLAST + 1)
#	define wxID_SHOWFILTERPROPERTIES (wxID_SHOWPINPROPERTIES + 1)
#endif

#ifdef __APPLE__
#	define wxID_DEVCONF 100
#endif

CameraCtrlFrame::CameraCtrlFrame(wxFrame *frame, const wxString& title, const wxPoint& pos,
			const wxSize& size, long style)
:OSGFrame(frame, title, pos, size, style)
{
#ifdef WIN32
	m_menubar = new wxMenuBar();

	m_devls = Util::getDeviceList(CLSID_VideoInputDeviceCategory);
	wxMenu *devmenu = new wxMenu();

	for (int i = 0;i < m_devls.size();i++)
	{
		devmenu->AppendRadioItem(wxID_DEVFIRST + i, wxString(Util::widen(m_devls[i].friendlyName.c_str())));
	}

	m_menubar->Append(devmenu, wxT("Devices"));

	Connect(wxID_DEVFIRST, wxID_DEVLAST, 
			wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(CameraCtrlFrame::OnSwitchDevices));

	wxMenu *optionMenu = new wxMenu();
	optionMenu->Append(wxID_SHOWPINPROPERTIES, wxT("Change Video Resolution"));
	optionMenu->Append(wxID_SHOWFILTERPROPERTIES, wxT("Adjust Video Properties"));
	
	m_menubar->Append(optionMenu, wxT("Options"));

	Connect(wxID_SHOWPINPROPERTIES, 
		wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(CameraCtrlFrame::OnShowPin));

	Connect(wxID_SHOWFILTERPROPERTIES, 
		wxEVT_COMMAND_MENU_SELECTED,
		wxCommandEventHandler(CameraCtrlFrame::OnShowFilter));

	SetMenuBar(m_menubar);
#endif
	
#ifdef __APPLE__
	m_menubar = new wxMenuBar();
	wxMenu *devmenu = new wxMenu();
	devmenu->Append(wxID_DEVCONF, wxT("Configure"));
	m_menubar->Append(devmenu, wxT("Devices"));
	
	Connect(wxID_DEVCONF, 
			wxEVT_COMMAND_MENU_SELECTED,
			wxCommandEventHandler(CameraCtrlFrame::OnDeviceConfig));

	SetMenuBar(m_menubar);
#endif
}


CameraCtrlFrame::~CameraCtrlFrame()
{
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
