#ifndef __UTIL_H__
#define __UTIL_H__

#include "arengine/Export"
#include "arengine/Exception.h"

#include <osg/Shader>

#ifdef __APPLE__
#	include <CoreFoundation/CFBundle.h>
#endif

#define boolToString(b) (b)?"true":"false"

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

		static bool loadShaderSource(osg::Shader* obj, const std::string& fileName );

	private:
		static unsigned int getFolderHash(string folder);
		static unsigned int getFileHash(string file);
		static unsigned int getConfHash(string file);
		static unsigned int hashFunc(const string &s);

#ifdef WIN32
	public:
		static wstring widen( const string& str );
		static string narrow( const wstring& str );

		static string getLocalAppDir();

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