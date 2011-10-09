#include "arengine/Util.h"
#include "arengine/Exception.h"
#include "arengine/Singleton.h"
#include "arengine/Logger.h"

#include <sys/stat.h>
#include <stdarg.h>

#include <string>
#include <sstream>

#ifdef WIN32
#	include <windows.h>
#	include <dshow.h>
#	include <stdio.h>
#	include <time.h>
#	include <vector>
#endif

#ifdef __APPLE__
#	include <dirent.h>
#	include <sys/time.h>
#endif


#ifdef WIN32
#	if _MSC_VER < 1400
#		define SSCANF sscanf
#	else
#		define SSCANF sscanf_s
#	endif
#else    
#	define SSCANF sscanf
#endif


using namespace std;
using namespace arengine;

#ifdef WIN32
	vector<DEVINFO> Util::devls;
#endif


int 
Util::makeInt(string s)
{
	const char *cstr;
	cstr = s.c_str();
	if (cstr != NULL)
	{
		// Hexadecimal
		if (cstr[0] == '0' && cstr[1] == 'x')
		{
			int hex;
			SSCANF(cstr+2, "%x", &hex);
			return hex;
		}
		else
		{
			return atoi(cstr);
		}
	}
	else
	{
		return 0;
	}


}


double
Util::makeDouble(string s)
{
	if (s.c_str())
	{
		return atof(s.c_str());
	}
	else
	{
		return 0.0;
	}
}


bool 
Util::makeBool(string s)
{
	if (!s.compare("true"))
	{
		return true;
	}
	else
	{
		return false;
	}
}


int
Util::getUniqueId()
{
	static int id = 0;
	id++;
	return id;
}


int 
Util::getLogLevel()
{
	Logger *logger = Singleton<Logger>::getInstance();
	return logger->getLogLevel();
}


void 
Util::setLogLevel(int level)
{
	if (level >= 1 && level <= 5)
	{
		Logger *logger = Singleton<Logger>::getInstance();
		logger->setLogLevel(level);
	}
}


void 
Util::log(Exception err)
{
	stringstream sstr;
	sstr << err.msg << Util::newLine();

	log(sstr.str(), err.logLevel);
}


void 
Util::log(string func, int logLevel, char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	char c_log[200];
	vsprintf(c_log, fmt, args);
	log(func, c_log, logLevel);
}


void 
Util::log(string func, string logMsg, int logLevel)
{
	stringstream sstr;
	sstr << func << ":" << logMsg;

	log(sstr.str(), logLevel);
}


void 
Util::log(string logMsg, int logLevel)
{
	stringstream sstr;
	sstr << logMsg << Util::newLine();

	Logger *logger = Singleton<Logger>::getInstance();
	logger->log(sstr.str(), logLevel);
	if (logLevel == 1)
	{
		logger->releaseLog();
		exit(1);
	}
}


int 
Util::getElapseTimeInMilliSec()
{
#ifdef WIN32
	return (int) clock();
#elif __APPLE__
	struct timeval t_now;
	gettimeofday(&t_now, NULL);

	int now = (int)((t_now.tv_sec * 1000) + (t_now.tv_usec / 1000));
	return now;
#endif
}


string 
Util::getNativePath(string path)
{
	string nativePath = path;
	size_t index = 0;
	while (true) {
		/* Locate the substring to replace. */
		index = nativePath.find("\\", index);
		if (index == string::npos) break;

		/* Make the replacement. */
		nativePath.replace(index, 1, "/");

		/* Advance index forward one spot so the next iteration doesn't pick it up as well. */
		++index;
	}

#ifdef __APPLE__
	nativePath = getBundleResourcePath(nativePath);
#endif

	return nativePath;
}


bool
Util::isValidImage()
{
	unsigned int imageHash;
	FILE *imgid = fopen("imgid", "r");
	if (imgid)
	{
		fscanf(imgid, "%u", &imageHash);
		return imageHash == getImageHash();
	}
	else
	{
		return false;
	}
}


unsigned int 
Util::getImageHash(string appPath)
{
	unsigned int confHash = getConfHash(appPath + "mastercv.conf");
	unsigned int modelHash = getFolderHash(appPath + "models");
	unsigned int hudHash = getFolderHash(appPath + "huds");
	unsigned int uiHash = getFolderHash(appPath + "ui");
	unsigned int soundHash = getFolderHash(appPath + "sounds");
	//printf("confHash = %u\n", confHash);
	//printf("modelHash = %u\n", modelHash);
	//printf("hudHash = %u\n", hudHash);
	//printf("uiHash = %u\n", uiHash);
	//printf("soundHash = %u\n", soundHash);
	return confHash + modelHash + hudHash + uiHash + soundHash;
}


unsigned int
Util::getFolderHash(string folder)
{
#ifdef WIN32
	bool working = true;
	wstring buffer;
	unsigned int folderHash = 0u;

	WIN32_FIND_DATA file;
	string searchLocation;
	searchLocation.append(folder);
	searchLocation.append("\\*.*");
	HANDLE hFolder=FindFirstFile(widen(searchLocation).c_str(), &file);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		buffer = file.cFileName;

		while(working)
		{
			FindNextFile(hFolder, &file);
			if(file.cFileName != buffer)
			{
				buffer = file.cFileName;
				if ((file.cFileName != wstring(L"."))
					&&(file.cFileName != wstring(L"..")))
				{
					string absPath;
					absPath.append(folder);
					absPath.append("\\");
					absPath.append(narrow(file.cFileName));
					folderHash += getFileHash(absPath);
					//printf("folder \"%s\" hash = %u\n", absPath.c_str(), getFileHash(absPath));
				}
			}
			else
			{
				//end of files reached
				working=false;
			}

		}
	}
	else {
		//Util::log(__FUNCTION__, 2, "Folder \"%s\" does not exist", folder.c_str());
	}

	return folderHash;
#endif

#ifdef __APPLE__
	unsigned int folderHash = 0u;
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(folder.c_str())) == NULL) {
		//Util::log(__FUNCTION__, 2, "Folder \"%s\" does not exist", folder.c_str());
		return folderHash;
	}

	while ((dirp = readdir(dp)) != NULL) {
		if (strcmp(dirp->d_name, ".") != 0 
			&& strcmp(dirp->d_name, "..") != 0)
		{
			string absPath;
			absPath.append(folder);
			absPath.append("/");
			absPath.append(dirp->d_name);
			folderHash += getFileHash(absPath);
			//printf("folder \"%s\" hash = %u\n", absPath.c_str(), getFileHash(absPath));
		}
	}
	closedir(dp);
	return folderHash;
#endif
}


unsigned int 
Util::getFileHash(string file)
{
	struct stat filestatus;
	stat(file.c_str(), &filestatus);
	return filestatus.st_size % 100000000;
}


unsigned int 
Util::getConfHash(string file)
{
	FILE *conf = fopen(file.c_str(), "r");
	string s("");
	if (conf)
	{
		while (!feof(conf))
		{
			char buff[256];
			fgets(buff, 256, conf);
			s.append(buff);
		}
	}
	fclose(conf);
	return hashFunc(s);
}


unsigned int 
Util::hashFunc(const string &s)
{
	unsigned int InitialFNV = 2166136261u;
	unsigned int FNVMultiple = 16777619u;
	unsigned int hash = InitialFNV;
	for(int i = 0; i < s.length(); i++)
	{
		hash = hash ^ (s[i]); /* xor the low 8 bits */
		hash = hash * FNVMultiple; /* multiply by the magic number */
	}
	return hash;
}


// Platform dependent

// WINDOWS
#ifdef WIN32


wstring 
Util::widen( const string& str )
{
	wostringstream wstm ;
	const ctype<wchar_t>& ctfacet = 
		use_facet< ctype<wchar_t> >( wstm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i ) 
		wstm << ctfacet.widen( str[i] ) ;
	return wstm.str() ;
}


string 
Util::narrow( const wstring& str )
{
	ostringstream stm ;
	const ctype<char>& ctfacet = 
		use_facet< ctype<char> >( stm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i ) 
		stm << ctfacet.narrow( str[i], 0 ) ;
	return stm.str() ;
}


HRESULT 
Util::enumerateDevices(REFGUID category, IEnumMoniker **ppEnum)
{
	// Create the System Device Enumerator.
	ICreateDevEnum *pDevEnum;
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,  
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void ** ) &pDevEnum);

	if (SUCCEEDED(hr))
	{
		// Create an enumerator for the category.
		hr = pDevEnum->CreateClassEnumerator(category, ppEnum, 0);
		if (hr == S_FALSE)
		{
			hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
		}
		pDevEnum->Release();
	}
	return hr;
}

void 
Util::getDeviceInformation(IEnumMoniker *pEnum, vector<DEVINFO> &devls)
{
	IMoniker *pMoniker = NULL;

	while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
	{
		IPropertyBag *pPropBag;
		HRESULT hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void ** ) &pPropBag);
		if (FAILED(hr))
		{
			pMoniker->Release();
			continue;  
		} 

		VARIANT var;
		VariantInit(&var);

		// Get description or friendly name.
		hr = pPropBag->Read(L"Description", &var, 0);
		if (FAILED(hr))
		{
			hr = pPropBag->Read(L"FriendlyName", &var, 0);
		}
		if (SUCCEEDED(hr))
		{
			//printf("%S\n", var.bstrVal);
			VariantClear(&var); 
		}

		hr = pPropBag->Write(L"FriendlyName", &var);

		DEVINFO devinfo;
		devinfo.friendlyName	=	narrow((LPCTSTR)var.bstrVal);
		pMoniker->BindToObject(0,0,IID_IBaseFilter, (void**)&(devinfo.pSrcFilter));

		devls.push_back(devinfo);
		pPropBag->Release();
		pMoniker->Release();
	}
}


vector<DEVINFO> 
Util::getDeviceList(REFGUID category)
{
	if (devls.empty())
	{
		IEnumMoniker *pEnum;
		HRESULT hr = enumerateDevices(CLSID_VideoInputDeviceCategory, &pEnum);
		getDeviceInformation(pEnum, devls);
		pEnum->Release();
	}
	return devls;
}


void 
Util::releaseDeviceList()
{
	int n = devls.size();
	for (int i = 0;i < n;i++)
	{
		devls[i].pSrcFilter.Release();
	}
}


#endif
// END (WINDOWS)