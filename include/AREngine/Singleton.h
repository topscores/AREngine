#ifndef __SINGLETON_H__
#define __SINGLETON_H__

namespace arengine
{	
	template<typename T>
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
	
	template <typename T> typename Singleton<T>::InstPtr Singleton<T>::sm_ptr;

}

#endif