#ifndef __OBJ_POOL_H__
#define __OBJ_POOL_H__

#include "arengine/Export"
#include "arengine/NamedObj.h"
#include "arengine/Action.h"
#include "arengine/SceneObj.h"
#include "arengine/Singleton.h"
#include "arengine/Util.h"

#include <osg/Referenced>
using namespace osg;

#include <vector>
#include <sstream>
using namespace std;


#define ActionPool arengine::ObjPool<arengine::Action>
#define SceneObjPool arengine::ObjPool<arengine::SceneObj>

namespace arengine
{

	template <class T>
	class ObjPool : public vector<ref_ptr<T>>
	{
	public:
		ObjPool();
		~ObjPool();

		void addObj(T *obj);
		ref_ptr<T> getByName(string name);

	};

}

using namespace arengine;
template <class T>
ObjPool<T>::ObjPool()
{
}


template <class T>
ObjPool<T>::~ObjPool()
{
}


template <class T>
void
ObjPool<T>::addObj(T *obj)
{
	if (obj != NULL)
	{
		string name = obj->getName();
		if (name == "")
		{
			push_back(obj);
		}
		else if(getByName(name) == NULL)
		{
			push_back(obj);
		}
		else
		{
			stringstream sstr;
			sstr << "Object \"" << obj->getName() << "\" is already in pool ";
			Util::log(__FUNCTION__, sstr.str().c_str(), 2);
		}
	}
}


template <class T>
ref_ptr<T>
ObjPool<T>::getByName(string name)
{
	int n = size();
	for (int i = 0;i < n;i++)
	{
		T *obj = this->at(i);
		if (obj != NULL)
		{
			if (obj->getName() == name)
			{
				stringstream sstr;
				sstr << "Find object " << name << " at position " << i;
				Util::log(__FUNCTION__, sstr.str().c_str(), 4);
				return obj;
			}
		}
	}
	return NULL;
}

#endif __OBJ_POOL_H__