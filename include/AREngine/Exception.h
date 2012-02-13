#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include "arengine/Export"
#include <string>

using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT Exception
	{
	public:
		Exception(string msg, int logLevel=3)
		{
			this->msg		= msg;
			this->logLevel	= logLevel;
		}

	public:
		string	msg;
		int		logLevel;
	};

}

#endif