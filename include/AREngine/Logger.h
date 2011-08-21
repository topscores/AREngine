#ifndef __LOGGER_H__
#define __LOGGER_H__

#ifdef WIN32
	#include <windows.h>
#endif 

#include "arengine/Export"
#include <string>

using namespace std;

namespace arengine
{

	// This class should be access via Singleton class
	class ARENGINE_EXPORT Logger
	{
	public:
		Logger();
		~Logger();

		void init(wstring fileName, int logLevel);
		void log(string logMsg, int logLevel);
		void setLogLevel(int logLevel);
		int  getLogLevel();
		void releaseLog();

	private:
		wstring getLogDir();
		
	private:
#ifdef WIN32
		HANDLE m_log;
#endif
		int		m_logLevel;
	};

}

#endif
