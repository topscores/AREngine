#ifndef __SMART_SINGLETON_H__
#define __SMART_SINGLETON_H__

//#include "arengine/Export"

#include <osg/Node>
using namespace osg;

namespace arengine
{	
	/*class ARScene;
	class KeyboardHandler;*/

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

	/*#ifndef IN_  // Note: For VC++, you can leave out the extern
		extern template class ARENGINE_EXPORT SmartSingleton<ARScene>;
		extern template class ARENGINE_EXPORT SmartSingleton<KeyboardHandler>;
	#endif*/
	
	template<class T>
	ref_ptr<T> SmartSingleton<T>::sm_ptr;

}

#endif