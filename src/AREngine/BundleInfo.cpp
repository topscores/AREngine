// Get resource location from application bundle
#ifdef __APPLE__

#include <string>
using namespace std;

#include "arengine/Util.h"
using namespace arengine;

CFBundleRef Util::mainBundle = 0;

bool
Util::initBundleLoading()
{
	mainBundle = CFBundleGetMainBundle();
	
	if(!mainBundle)
	{
		Util::log(__FUNCTION__, 1, "Failed to load bundle!");
		return false;
	}
	
	return true;
}


string
Util::getBundleResourcePath(string fileName)
{
	if (!mainBundle)
	{
		initBundleLoading();
	}
	
	CFURLRef resourceURL;
	
	// Get resource directory
	resourceURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	if(!resourceURL)
	{
		Util::log(__FUNCTION__, 1, "Failed to locate %s in the loaded bundle!", fileName.c_str());
	}
	
	char *fileurl = new char [200];
	
	
	if(!CFURLGetFileSystemRepresentation(
										 resourceURL,
										 true,
										 (UInt8*)
										 fileurl,
										 200))
		
	{
		Util::log(__FUNCTION__, 1, "Failed to turn %s into a filesystem path representation!", fileName.c_str());
	}

	else
	{
		string nativePath;
		nativePath.append(fileurl);
		nativePath.append("/");
		nativePath.append(fileName);
		return nativePath;
	}	
}

#endif