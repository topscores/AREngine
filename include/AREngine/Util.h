#ifndef __UTIL_H__
#define __UTIL_H__

#include "arengine/Export"
#include "arengine/Exception.h"
#include "arengine/Singleton.h"
#include "arengine/Logger.h"

#include <time.h>
#include <stdarg.h>

#include <string>
#include <sstream>

using namespace std;

#ifdef WIN32
	#define SSCANF sscanf_s
#else    
	#define SSCANF sscanf
#endif

#ifdef __APPLE__
	#include <CoreFoundation/CFBundle.h>
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


		static int getElapseTimeInMilliSec()
		{
			return (int) clock();
		}


		static int getCurrentTime()
		{
			return (int) time(NULL);
		}
		

		static string getNativePath(string path)
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
		
/*		static bool isPath(string s)
		{
			int i;
			double f;
			i = atoi(s.c_str());
			
			// If it is not a numeric value
			if (i != 0)
			{
				int index = s.find(".", 0);
			
				// There is no "." , assume this is not a file path
				if (index == string::npos)
				{
					return false;
				}
				else 
				{
					f = atof(s.c_str());
					return true;
				}
			}
			else 
			{
				return false;
			}

		}*/
		
#ifdef WIN32
		static wstring widen( const string& str )
		{
			wostringstream wstm ;
			const ctype<wchar_t>& ctfacet = 
			use_facet< ctype<wchar_t> >( wstm.getloc() ) ;
			for( size_t i=0 ; i<str.size() ; ++i ) 
				wstm << ctfacet.widen( str[i] ) ;
			return wstm.str() ;
		}
		
		static string narrow( const wstring& str )
		{
			ostringstream stm ;
			const ctype<char>& ctfacet = 
			use_facet< ctype<char> >( stm.getloc() ) ;
			for( size_t i=0 ; i<str.size() ; ++i ) 
				stm << ctfacet.narrow( str[i], 0 ) ;
			return stm.str() ;
		}
#endif

#ifdef __APPLE__
	private:
		static CFBundleRef mainBundle;

	public:
		static bool initBundleLoading();
		static string getBundleResourcePath(string fileName);
#endif

	};
}
#endif