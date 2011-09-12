#ifndef __UTIL_H__
#define __UTIL_H__

#ifdef WIN32
#include <Windows.h>
#include <stdio.h>
#endif

#include "arengine/Export"
#include "arengine/Exception.h"
#include "arengine/Singleton.h"
#include "arengine/Logger.h"

#include <time.h>
#include <direct.h>
#include <sys/stat.h>
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
	public:
		// Check for program integrity
		static bool isValidImage()
		{
			int imageHash;
			FILE *imgid = fopen("imgid", "r");
			if (imgid)
			{
				fscanf(imgid, "%d", &imageHash);
				return imageHash == getImageHash();
			}
			else
			{
				return false;
			}
		}

		static int getImageHash(string appPath = "")
		{
			int confHash = getConfHash(appPath + "mastercv.conf");
			int modelHash = getFolderHash(appPath + "models");
			int hudHash = getFolderHash(appPath + "huds");
			int uiHash = getFolderHash(appPath + "ui");
			int soundHash = getFolderHash(appPath + "sounds");
			return confHash + modelHash + hudHash + uiHash + soundHash;
		}

	private:
		static int getFolderHash(string folder)
		{
#ifdef WIN32
			bool working = true;
			wstring buffer;
			int folderHash = 0;

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
						string absPath;
						absPath.append(folder);
						absPath.append("\\");
						absPath.append(narrow(file.cFileName));
						folderHash += getFileHash(absPath);
					}
					else
					{
						//end of files reached
						working=false;
					}

				}
			}
			return folderHash;
#endif

		}

		static int getFileHash(string file)
		{
			struct stat filestatus;
			stat(file.c_str(), &filestatus);
			return filestatus.st_size % 100000000;
		}

		static int getConfHash(string file)
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

		static int hashFunc(const string &s)
		{
			int InitialFNV = 2166136261;
			int FNVMultiple = 16777619;
			int hash = InitialFNV;
			for(int i = 0; i < s.length(); i++)
			{
				hash = hash ^ (s[i]); /* xor the low 8 bits */
				hash = (hash * FNVMultiple)%100000000; /* multiply by the magic number */
			}
			return hash;
		}
	};
}
#endif