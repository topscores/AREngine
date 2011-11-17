#include "arengine/CaptureDeviceManager.h"
#include "arengine/Util.h"

#include <sstream>

#ifdef WIN32
#	define VALID_ID(id) (id >= 0 && id < getDeviceCount())
#	define XMLTEMPLATE_FILE "Data\\WDM_camera_template.xml"
#	define INI_FILENAME "\\Larngear\\arengine.ini"
#	define INI_APPNAME "ARENGINE"
#endif

using namespace arengine;
#ifdef WIN32
	bool CaptureDeviceManager::initialized = false;
	std::vector<DEVINFO> CaptureDeviceManager::m_devls;
#endif


string
CaptureDeviceManager::createConfigString(bool showFormatDialog,
										 bool mirrorMode,
										 int width,
										 int height)
{
	string fmt;
#ifdef WIN32
	char szBuf[1000], szFmt[1000];
	readXMLTemplate(szBuf);

	bool flip_h = mirrorMode;
	bool flip_v = true;
	char fw[50], fh[50];
	if (width > 0 && height > 0)
	{
		sprintf(fw, "frame_width=\"%d\"", width);
		sprintf(fh, "frame_height=\"%d\"", height);
	}
	else
	{
		fw[0]=0;
		fh[0]=0;
	}

	sprintf(szFmt, szBuf, boolToString(showFormatDialog), 
		fw, fh, boolToString(flip_h), boolToString(flip_v));
	fmt = string(szFmt);
#endif

#ifdef __APPLE__
	if (!mirrorMode)
	{
		if (!showFormatDialog)
		{
			fmt = string("-nodialog");
		}
		else
		{
			fmt = string("");
		}
	}
	else
	{
		if (!showFormatDialog)
		{
			fmt= string("-nodialog -fliph");
		}
		else
		{
			fmt = string("-fliph");
		}
	}
#endif
	return fmt;
}


#ifdef WIN32

void
CaptureDeviceManager::loadCapDevInfo(string &displayName,
									 int &frame_width,
									 int &frame_height)
{
	string inifile;
	inifile = Util::getLocalAppDir();
	inifile.append(INI_FILENAME);

	char capdev[200];

	GetPrivateProfileStringA(INI_APPNAME, "capdev", "", capdev, sizeof(capdev), inifile.c_str());
	
	displayName = string(capdev);
	frame_width = GetPrivateProfileIntA(INI_APPNAME, "frame_width", 0, inifile.c_str());
	frame_height = GetPrivateProfileIntA(INI_APPNAME, "frame_height", 0, inifile.c_str());
}


void
CaptureDeviceManager::saveCapDevInfo(string displayName, int frame_width, int frame_height)
{
	populateDeviceList();

	char szFrameWidth[10], szFrameHeight[10];
	sprintf(szFrameWidth, "%d", frame_width);
	sprintf(szFrameHeight, "%d", frame_height);

	string inifile;
	inifile = Util::getLocalAppDir();
	inifile.append(INI_FILENAME);
	WritePrivateProfileStringA(INI_APPNAME, "capdev", displayName.c_str(), inifile.c_str());
	WritePrivateProfileStringA(INI_APPNAME, "frame_width", szFrameWidth, inifile.c_str());
	WritePrivateProfileStringA(INI_APPNAME, "frame_height", szFrameHeight, inifile.c_str());
}


int
CaptureDeviceManager::getDeviceCount()
{
	populateDeviceList();
	return m_devls.size();
}


string
CaptureDeviceManager::getFriendlyName(int devid)
{
	populateDeviceList();
	if (VALID_ID(devid))
	{
		return m_devls[devid].friendlyName;
	}
	else
	{
		return "";
	}
}


string
CaptureDeviceManager::getDisplayName(int devid)
{
	populateDeviceList();
	if (VALID_ID(devid))
	{
		return m_devls[devid].displayName;
	}
	else
	{
		return "";
	}
}


CComPtr<IBaseFilter>
CaptureDeviceManager::getAsSourceFilter(int devid)
{
	populateDeviceList();
	if (VALID_ID(devid))
	{
		return m_devls[devid].pSrcFilter;
	}
	else
	{
		return NULL;
	}
}


CComPtr<IBaseFilter>
CaptureDeviceManager::getAsSourceFilter(string displayName)
{
	populateDeviceList();
	int n  = getDeviceCount();
	for (int i = 0;i < n;i++)
	{
		if (m_devls[i].displayName == displayName)
		{
			return m_devls[i].pSrcFilter;
		}
	}
	return NULL;
}


void
CaptureDeviceManager::populateDeviceList()
{
	if (!initialized)
	{
		CComPtr<IEnumMoniker> pEnum;
		HRESULT hr = enumerateDevices(&pEnum);
		if (SUCCEEDED(hr))
		{
			getDeviceInformation(pEnum);
		}
		else
		{
			Util::log(__FUNCTION__, 1, "Error getting device information error code = %d", GetLastError());
		}
		initialized = true;
	}
}


HRESULT 
CaptureDeviceManager::enumerateDevices(IEnumMoniker **ppEnum)
{
	// Create the System Device Enumerator.
	CComPtr<ICreateDevEnum> pDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,  
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void ** ) &pDevEnum);

	if (SUCCEEDED(hr))
	{
		// Create an enumerator for the category.
		hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, ppEnum, 0);
		if (hr == S_FALSE)
		{
			hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
		}
	}
	return hr;
}

void 
CaptureDeviceManager::getDeviceInformation(IEnumMoniker *pEnum)
{
	CComPtr<IMoniker> pMoniker = NULL;

	while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
	{
		CComPtr<IPropertyBag> pPropBag;
		HRESULT hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void ** ) &pPropBag);
		if (FAILED(hr))
		{
			continue;  
		} 

		VARIANT var;
		VariantInit(&var);

		// Get description or friendly name.
		string friendlyName;
		hr = pPropBag->Read(L"Description", &var, 0);
		if (FAILED(hr))
		{
			hr = pPropBag->Read(L"FriendlyName", &var, 0);
		}
		if (SUCCEEDED(hr))
		{
			friendlyName = Util::narrow((LPCTSTR)var.bstrVal);
			VariantClear(&var); 
		}

		WCHAR *szBuf;
		char displayName[200];
		pMoniker->GetDisplayName(NULL, NULL, &szBuf);
		if (szBuf)
		{
			USES_CONVERSION;
			strncpy(displayName, W2A(szBuf), NUMELMS(displayName));
			CoTaskMemFree(szBuf);
		}

		DEVINFO devinfo;
		devinfo.friendlyName	=	friendlyName;
		devinfo.displayName		=	string(displayName);
		pMoniker->BindToObject(0,0,IID_IBaseFilter, (void**)&(devinfo.pSrcFilter));

		m_devls.push_back(devinfo);
	}
}


void
CaptureDeviceManager::readXMLTemplate(char szBuf[1000])
{
	FILE *fTemplate = fopen(XMLTEMPLATE_FILE, "r");
	char buf[100];
	stringstream sstr;
	while(!feof(fTemplate))
	{
		if (fgets(buf, sizeof(buf), fTemplate) != NULL)
		{
			sstr << buf;
		}
	}
	fclose(fTemplate);
	sprintf(szBuf, "%s", sstr.str().c_str());
}


#endif // WIN32

