#include "arengine/MarkerPool.h"
#include "arengine/Util.h"

#include "arengine/ARScene.h"
#include "arengine/SmartSingleton.h"

using namespace arengine;

MarkerPool::MarkerPool()
{
	m_tracker = SmartSingleton<ARScene>::getInstance()->getTracker();
}


MarkerPool::~MarkerPool()
{
}


bool
MarkerPool::hasMarker(string initString)
{
	map< string, ref_ptr< osgART::Marker > >::iterator itr;
	itr = m_markerpool.find(initString);
	return itr != m_markerpool.end();
}


ref_ptr<osgART::Marker>
MarkerPool::getMarker(string initString)
{
	map< string, ref_ptr< osgART::Marker > >::iterator itr;
	itr = m_markerpool.find(initString);

	// Already has marker
	if (itr != m_markerpool.end())
	{
		return  itr->second;
	}
	else
	{
		osgART::Marker* marker = m_tracker->addMarker(initString);
		m_markerpool[initString] = marker;
		return marker;
	}
}