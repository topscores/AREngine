#ifndef __UTIL_H__
#define __UTIL_H__

#include <windows.h>
#include "arengine/Export"
#include "arengine/Exception.h"
#include "arengine/Singleton.h"
#include "arengine/Logger.h"

#include "time.h"
#include <string>
#include <sstream>

using namespace std;

#ifdef _UNICODE 
typedef wstring tstring;
#else
typedef string tstring;
#endif


namespace arengine
{

	class ARENGINE_EXPORT Util
	{
	public:
		static int makeInt(string s)
		{
			if (s.c_str())
			{
				return atoi(s.c_str());
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
			//string errMsg;
			//errMsg.append(err.msg);
			//errMsg.append(1, Util::newLine());

			stringstream sstr;
			sstr << err.msg << Util::newLine();

			log(sstr.str(), err.logLevel);
		}
		

		static void log(string func, string logMsg, int logLevel = 3)
		{
			stringstream sstr;
			sstr << func << ":" << logMsg << Util::newLine();
			
			log(sstr.str(), logLevel);
		}


		static void log(string logMsg, int logLevel = 3)
		{
			//string msg;
			//msg.append(logMsg);
			//msg.append(1, Util::newLine());

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
			clock_t curClock = clock();
			double second = ((double)curClock)/((double)CLOCKS_PER_SEC);
			int milliSec = (int) second * 1000;
			return milliSec;
		}

	};
}
#endif