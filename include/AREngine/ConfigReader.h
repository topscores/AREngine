#ifndef __CONFIG_READER_H__
#define __CONFIG_READER_H__

#include "arengine/Export"

#include <string>
#include "arengine/DataNode.h"

using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT ConfigReader
	{
	public:
		ConfigReader(){}
		virtual ~ConfigReader(){}

		// Caller must take responsibility for releasing resulted DataNode*
		virtual DataNode* readConfigFile(string fileName) = 0;
	};

}

#endif