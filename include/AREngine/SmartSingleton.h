#ifndef __SMART_SINGLETON_H__
#define __SMART_SINGLETON_H__

#include <osg/Node>
using namespace osg;

namespace arengine
{	
	template<typename T>
	class SmartSingleton
	{
	private:
		class InstPtr
		{
		public:
			InstPtr() {m_ptr = 0;}
			~InstPtr() {}
			ref_ptr<T> Get() { return m_ptr; }
			void Set(T* p)
			{
				if(p!= 0)
				{
					m_ptr = p;
				}
			}
		private:
			ref_ptr<T> m_ptr;
		};
		
		static InstPtr sm_ptr;
		SmartSingleton();
		SmartSingleton(const SmartSingleton&);
		SmartSingleton& operator=(const SmartSingleton&);

	public:
		static ref_ptr<T> getInstance()
		{
			if(!sm_ptr.Get().valid())
			{
				sm_ptr.Set(new T());
			}
			return sm_ptr.Get();
		}
	};
	
	template <typename T> typename SmartSingleton<T>::InstPtr SmartSingleton<T>::sm_ptr;

}

#endif