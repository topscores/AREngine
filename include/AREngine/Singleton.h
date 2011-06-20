#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "arengine/Export"

namespace arengine
{

	template<class T>
	class ARENGINE_EXPORT Singleton
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

}

#endif