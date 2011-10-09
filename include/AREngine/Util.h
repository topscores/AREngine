#ifndef __UTIL_H__
#define __UTIL_H__

#include "arengine/Export"
#include "arengine/Exception.h"

#ifdef WIN32
#	include <windows.h>
#	include <dshow.h>
#	include <atlbase.h>
#	include <vector>
#endif

#ifdef __APPLE__
#	include <CoreFoundation/CFBundle.h>
#endif


#define boolToString(b) (b)?"true":"false"


#ifdef WIN32
typedef struct  tagDEVINFO{
							string friendlyName;
							CComPtr<IBaseFilter> pSrcFilter;
							}	DEVINFO;
#endif

namespace arengine
{

	class ARENGINE_EXPORT Util
	{
	public:
		static int makeInt(string s);
		static double makeDouble(string s);
		static bool makeBool(string s);

		static char newLine(){return '\n';}
		static int getUniqueId();

		static int getLogLevel();
		static void setLogLevel(int level);

		static void log(Exception err);
		static void log(string func, int logLevel, char *fmt, ...);
		static void log(string func, string logMsg, int logLevel = 3);
		static void log(string logMsg, int logLevel = 3);

		static int getElapseTimeInMilliSec();
		static string getNativePath(string path);

		// Check for program integrity
		static bool isValidImage();
		static unsigned int getImageHash(string appPath = "");

	private:
		static unsigned int getFolderHash(string folder);
		static unsigned int getFileHash(string file);
		static unsigned int getConfHash(string file);
		static unsigned int hashFunc(const string &s);

#ifdef WIN32
	public:
		static wstring widen( const string& str );
		static string narrow( const wstring& str );

		static vector<DEVINFO> getDeviceList(REFGUID category);
		static void releaseDeviceList();
	
	private:
		static HRESULT enumerateDevices(REFGUID category, IEnumMoniker **ppEnum);
		static void getDeviceInformation(IEnumMoniker *pEnum, vector<DEVINFO> &devls);

	private:
		static std::vector<DEVINFO> devls;

#endif

#ifdef __APPLE__
	public:
		static bool initBundleLoading();
		static string getBundleResourcePath(string fileName);

	private:
		static CFBundleRef mainBundle;
#endif

	};
}
#endif