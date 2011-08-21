#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "arengine/Export"

#define ActionPool arengine::ObjPool<arengine::Action>
#define SceneObjPool arengine::ObjPool<arengine::SceneObj>

namespace arengine
{	
	class Logger;
	class Config;

	// The definitions of ActionPool and SceneObjPool needs these classes
	template<class T> class ObjPool;
	class Action;
	class SceneObj;
	//

	class SDLSoundManager;

	template<class T>
	class Singleton
	{
	private:		
		class InstPtr
		{
		public:
			InstPtr() : m_ptr(0) {}
			~InstPtr() { delete m_ptr; }
			T* Get() { return m_ptr; }
			void Set(T* p)
			{
				if(p!= 0)
				{
					delete m_ptr;
					m_ptr = p;
				}
			}
		private:
			T* m_ptr;
		};
		
		static InstPtr sm_ptr;
		Singleton();
		Singleton(const Singleton&);
		Singleton& operator=(const Singleton&);

	public:
		static T* getInstance()
		{
			if(sm_ptr.Get() == 0)
			{
				sm_ptr.Set(new T());
			}
			return sm_ptr.Get();
		}
	};

	#ifndef IN_  // Note: For VC++, you can leave out the extern
		extern template class ARENGINE_EXPORT Singleton<Logger>;
		extern template class ARENGINE_EXPORT Singleton<Config>;
		extern template class ARENGINE_EXPORT Singleton<ActionPool>;
		extern template class ARENGINE_EXPORT Singleton<SceneObjPool>;
		extern template class ARENGINE_EXPORT Singleton<SDLSoundManager>;
	#endif
	
}

#endif