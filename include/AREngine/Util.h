#ifndef __UTIL_H__
#define __UTIL_H__

#include <windows.h>
#include "arengine/Export"
#include "arengine/Exception.h"
#include "arengine/Singleton.h"
#include "arengine/Logger.h"

#include <time.h>
#include <stdarg.h>

#include <string>
#include <sstream>

using namespace std;

#ifdef _UNICODE 
typedef wstring tstring;
#else
typedef string tstring;
#endif


#define boolToString(b) (b)?"true":"false"
namespace arengine
{

	class ARENGINE_EXPORT Util
	{
	public:

		static int makeInt(string s)
		{
			const char *cstr;
			cstr = s.c_str();
			if (cstr != NULL)
			{
				// Hexadecimal
				if (cstr[0] == '0' && cstr[1] == 'x')
				{
					int hex;
					sscanf_s(cstr+2, "%x", &hex);
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

		static double makeDouble(string s)
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

		static bool makeBool(string s)
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

		static char newLine()
		{
			return '\n';
		}

		static int getUniqueId()
		{
			static int id = 0;
			id++;
			return id;
		}

		static void playSound(tstring fileName, bool loop)
		{
			if (!fileName.empty())
			{
				if (loop)
				{
					PlaySound(fileName.c_str(), NULL,SND_FILENAME|SND_LOOP|SND_ASYNC);
				}
				else
				{
					PlaySound(fileName.c_str(), NULL,SND_FILENAME|SND_ASYNC);
				}
			}
		}


		static void stopSound(tstring fileName)
		{
			if (!fileName.empty())
			{
				PlaySound(fileName.c_str(), NULL, SND_ASYNC | SND_PURGE);
			}
		}


		static void stopAllSound()
		{
			PlaySound(NULL, NULL, SND_ASYNC | SND_PURGE);
		}


		static int getLogLevel()
		{
			Logger *logger = Singleton<Logger>::getInstance();
			return logger->getLogLevel();
		}

		static void setLogLevel(int level)
		{
			if (level >= 1 && level <= 5)
			{
				Logger *logger = Singleton<Logger>::getInstance();
				logger->setLogLevel(level);
			}
		}


		static void log(Exception err)
		{
			stringstream sstr;
			sstr << err.msg << Util::newLine();

			log(sstr.str(), err.logLevel);
		}

		static void log(string func, int logLevel, char *fmt, ...)
		{
			va_list args;
			va_start(args, fmt);

			char c_log[200];
			vsprintf(c_log, fmt, args);
			log(func, c_log, logLevel);
		}

		static void log(string func, string logMsg, int logLevel = 3)
		{
			stringstream sstr;
			sstr << func << ":" << logMsg;

			log(sstr.str(), logLevel);
		}

		static void log(string logMsg, int logLevel = 3)
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

		static int getTimeInMilliSec()
		{
			time_t milliSec = time(NULL) * 1000;
			return (int) milliSec;
		}

	};
}
#endif