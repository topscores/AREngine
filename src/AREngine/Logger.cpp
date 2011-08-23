#ifdef WIN32
	#include <Shlobj.h>
	#include <arengine/Util.h>	
#endif

#ifdef __APPLE__
	#include <CoreServices/CoreServices.h>
	#include <CoreFoundation/CoreFoundation.h>
	#include <unistd.h>
	#include <dlfcn.h>
	#include "arengine/Arengine.h"
#endif

#include "arengine/Logger.h"
using namespace arengine;


Logger::Logger():
m_log(NULL),	// Not yet initialized
m_logLevel(-1)	// Will not log anything
{
}


Logger::~Logger()
{
	releaseLog();
}


void
Logger::init(string fileName, int logLevel, string appName)
{
	if (logLevel >= 1 && logLevel <= 5)
	{
		m_logLevel	= logLevel;
	}
	else
	{
		// Default log level = 3
		m_logLevel = 3;
	}

	string logPath;
	logPath.append(getLogDir(appName));
	logPath.append(fileName);

	m_log = fopen(logPath.c_str(), "w");
}


void
Logger::log(string logMsg, int logLevel)
{
	if (m_log == NULL) // Have not initialized yet, log to console
	{
		printf(logMsg.c_str());
	}
	else if (logLevel <= m_logLevel)
	{
		fprintf(m_log, "%s", logMsg.c_str());
	}
}


void
Logger::setLogLevel(int logLevel)
{
	if (logLevel >= 1 && logLevel <= 5)
	{
		m_logLevel = logLevel;
	}
}


int
Logger::getLogLevel()
{
	return m_logLevel;
}


void
Logger::releaseLog()
{
	fclose(m_log);

	m_log		= NULL;
	m_logLevel	= -1;
}


string
Logger::getLogDir(string appName)
{
#ifdef WIN32
	HRESULT res;
	wchar_t appDataDir[MAX_PATH], moduleFileName[MAX_PATH];
	
	res = SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataDir);
	if (res == E_FAIL) 
	{
		return "";
	}

	wstring logDir;
	logDir.append(appDataDir);
	logDir.append(L"\\Larngear\\");
	logDir.append(Util::widen(appName));
	logDir.append(L"\\");
	SHCreateDirectory(NULL, logDir.c_str());
	return Util::narrow(logDir);
#endif
	
#ifdef __APPLE__
	char homePath[1024];
	FSRef homeRef;
	FSFindFolder(kOnAppropriateDisk, kCurrentUserFolderType, kCreateFolder, &homeRef);
	FSRefMakePath(&homeRef,(UInt8*) &homePath ,sizeof(homePath) );
	
	string logDir;
	logDir.append(homePath);
	if (!logDir.empty())
	{
		logDir.append("/");
		logDir.append("Library/Logs/Larngear");
		logDir.append("/");
	}
	else 
	{
		return "";
	}
	
	logDir.append(appName);
	logDir.append("/");
	
	string cmd;
	cmd.append("mkdir -p ");
	cmd.append(logDir);
	system(cmd.c_str());
	
	return logDir;

#endif
	
	return "";
}
