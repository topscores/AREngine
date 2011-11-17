#ifndef __CAPTURE_DEVICE_MANAGER_H__
#define __CAPTURE_DEVICE_MANAGER_H__

#ifdef WIN32
#	include <windows.h>
#	include <vector>
#	include <dshow.h>
#	include <atlbase.h>
#	include <string>
#endif

#include "arengine/Export"
#include "arengine/Singleton.h"

using namespace std;

#ifdef WIN32
typedef struct  tagDEVINFO{
	string friendlyName;
	string displayName;
	CComPtr<IBaseFilter> pSrcFilter;
}	DEVINFO;
#endif

namespace arengine
{
	class ARENGINE_EXPORT CaptureDeviceManager
	{
		template<class CaptureDeviceManager> friend class Singleton;

	public:
		static string createConfigString(bool showFormatDialog, 
			bool mirrorMode, 
			int width=0,
			int height=0);
#ifdef WIN32
	public:
		static void   loadCapDevInfo(string &displayName,
			int &frame_width,
			int &frame_height);
		static void   saveCapDevInfo(string displayName,
			int frame_width,
			int frame_height);

		static int getDeviceCount();
		static string getFriendlyName(int devid);
		static string getDisplayName(int devid);

		static CComPtr<IBaseFilter> getAsSourceFilter(int devid);
		static CComPtr<IBaseFilter> getAsSourceFilter(string displayName);

	private:
		static void populateDeviceList();
		static HRESULT enumerateDevices(IEnumMoniker **ppEnum);
		static void getDeviceInformation(IEnumMoniker *pEnum);

		static void readXMLTemplate(char szBuf[1000]);

	private:
		static bool initialized;
		static std::vector<DEVINFO> m_devls;
#endif

#ifdef __APPLE__
#endif
	};
}

#endif