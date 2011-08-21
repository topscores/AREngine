#ifdef WIN32
	#include <Shlobj.h>
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
Logger::init(wstring fileName, int logLevel)
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

	wstring logPath;
	logPath.append(getLogDir());
	logPath.append(fileName);

#ifdef WIN32
	DWORD err;
	m_log	= CreateFile(logPath.c_str(), FILE_WRITE_DATA, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	err = GetLastError();
#endif
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
#ifdef WIN32
		DWORD n;
		WriteFile(m_log, logMsg.c_str(), logMsg.size(), &n, NULL);
#endif
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
#ifdef WIN32
	if (m_log)
	{
		CloseHandle(m_log);

		m_log		= NULL;
		m_logLevel	= -1;
	}
#endif
}


wstring
Logger::getLogDir()
{
#ifdef WIN32
	wchar_t appDataDir[MAX_PATH], moduleFileName[MAX_PATH];
	
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataDir);
	
	GetModuleFileName(NULL, moduleFileName, MAX_PATH);
	wstring moduleName;
	int ext_begin;
	int module_begin;
	moduleName.append(moduleFileName);
	module_begin = moduleName.find_last_of(L"\\") + 1;
	ext_begin = moduleName.find_last_of(L".");
	moduleName = moduleName.substr(module_begin, ext_begin - module_begin);

	wstring logDir;
	logDir.append(appDataDir);
	logDir.append(L"\\Larngear\\");
	logDir.append(moduleName);
	logDir.append(L"\\");
	SHCreateDirectory(NULL, logDir.c_str());
	return logDir;
#endif
}
