#ifndef __SMART_SINGLETON_H__
#define __SMART_SINGLETON_H__

#include "arengine/Export"

#include <osg/Node>
using namespace osg;


#include "arengine/KeyboardHandler.h"
#include "arengine/ARScene.h"


namespace arengine
{
	extern "C" 
	{
		extern ARENGINE_EXPORT ref_ptr<ARScene> g_arscene;
		extern ARENGINE_EXPORT ref_ptr<KeyboardHandler> g_kbhdl;
	}

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
	
/*	template<>
	class SmartSingleton<ARScene>
	{
	private:
		SmartSingleton();
		SmartSingleton(const SmartSingleton&);
		SmartSingleton& operator=(const SmartSingleton&);
		
	public:
		static ARScene* getInstance()
		{
			if(!g_arscene.valid())
			{
				g_arscene = new ARScene();
			}
			return g_arscene.get();
		}
	};
	
	template<>
	class SmartSingleton<KeyboardHandler>
	{
	private:
		SmartSingleton();
		SmartSingleton(const SmartSingleton&);
		SmartSingleton& operator=(const SmartSingleton&);
		
	public:
		static KeyboardHandler* getInstance()
		{
			if(!g_kbhdl.valid())
			{
				g_kbhdl = new KeyboardHandler();
			}
			return g_kbhdl.get();
		}
	};*/
	
	template<class T>
	ref_ptr<T> SmartSingleton<T>::sm_ptr;

}

#endif