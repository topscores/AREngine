#ifndef __SMART_SINGLETON_H__
#define __SMART_SINGLETON_H__

#include <osg/Node>
using namespace osg;

namespace arengine
{	
	template<class T>
	class SmartSingleton
	{
	private:
		static ref_ptr<T> sm_ptr;
		SmartSingleton();
		SmartSingleton(const SmartSingleton&);
		SmartSingleton& operator=(const SmartSingleton&);

	public:
		static T* getInstance()
		{
			if(!sm_ptr.valid())
			{
				sm_ptr = new T();
			}
			return sm_ptr.get();
		}
	};
	
	template<class T> 
	ref_ptr<T> SmartSingleton<T>::sm_ptr;

}

#endif