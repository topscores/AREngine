#ifndef __SMART_SINGLETON_H__
#define __SMART_SINGLETON_H__

#include "arengine/Export"

#include <osg/Node>
using namespace osg;

namespace arengine
{
	template<class T>
	class ARENGINE_EXPORT SmartSingleton
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
}

#endif