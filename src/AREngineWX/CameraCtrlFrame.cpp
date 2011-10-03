#include "arengine/AREngine.h"
#include "arengine/ARScene.h"
#include "arengine/Util.h"
#include "arenginewx/CameraCtrlFrame.h"

using namespace arengine;
using namespace arenginewx;

#define MAX_DEV 10
#define wxID_DEVFIRST 0
#define wxID_DEVLAST (MAX_DEV-1)
#define wxID_SHOWPINPROPERTIES (MAX_DEV)
#define wxID_SHOWFILTERPROPERTIES (MAX_DEV+1)

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
		devmenu->AppendRadioItem(i, wxString(Util::widen(m_devls[i].friendlyName.c_str())));
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
}


CameraCtrlFrame::~CameraCtrlFrame()
{
}

#ifdef _WIN32

void
CameraCtrlFrame::OnSwitchDevices(wxCommandEvent &event)
{
	int devid = event.GetId();
	ARScene *arscene = AREngine::getARScene();
	if (arscene)
	{
		arscene->changeCaptureDevice(m_devls[devid].pSrcFilter);
	}
}


void
CameraCtrlFrame::OnShowPin(wxCommandEvent &event)
{
	ARScene *arscene = AREngine::getARScene();
	if (arscene)
	{
		arscene->showPinProperties();
	}
}


void
CameraCtrlFrame::OnShowFilter(wxCommandEvent &event)
{
	ARScene *arscene = AREngine::getARScene();
	if (arscene)
	{
		arscene->showFilterProperties();
	}
}

#endif
