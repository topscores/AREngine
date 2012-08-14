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


ref_ptr<osgART::TransformFilterCallback>
MarkerPool::getSmoothFilterForMarker(std::string initString)
{
	map< string, ref_ptr< osgART::TransformFilterCallback > >::iterator itr;
	itr = m_smoothFilterpool.find(initString);
	if (itr != m_smoothFilterpool.end())
	{
		return  itr->second;
	}
	else
	{
		osgART::TransformFilterCallback* tfc = new osgART::TransformFilterCallback();
		tfc->enableTranslationalSmoothing(true);
		tfc->enableRotationalSmoothing(true);

		m_smoothFilterpool[initString] = tfc;

		return tfc;
	}
}