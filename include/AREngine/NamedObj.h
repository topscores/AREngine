#ifndef __NAMED_OBJ_H__
#define __NAMED_OBJ_H__

#include "arengine/Export"

#include <string>
using namespace std;

namespace arengine
{
	
	class ARENGINE_EXPORT NamedObj
	{
	public:
		virtual string getName() = 0;
	};

}

#endif