#ifndef __SMART_SINGLETON_H__
#define __SMART_SINGLETON_H__

#include "arengine/Export"

#include <osg/Node>

#include "arengine/KeyboardHandler.h"
#include "arengine/ARScene.h"


namespace arengine
{

	template<class T>
	class SmartSingleton
	{
	private:
		static osg::ref_ptr<T> sm_ptr;
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
	osg::ref_ptr<T> SmartSingleton<T>::sm_ptr;

}

#endif