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

		void init(string fileName, int logLevel, string appName);
		void log(string logMsg, int logLevel);
		void setLogLevel(int logLevel);
		int  getLogLevel();
		void releaseLog();

	private:
		string getLogDir(string appName);
		
	private:
		FILE	*m_log;
		int		m_logLevel;
	};

}

#endif
