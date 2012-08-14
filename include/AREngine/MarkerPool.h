#ifndef __MARKER_POOL_H__
#define __MARKER_POOL_H__

#include "arengine/Export"
#include "arengine/Singleton.h"

#include <osg/Node>

#include <osgART/Tracker>
#include <osgART/TransformFilterCallback>

#include <map>
#include <string>

using namespace osg;

namespace arengine
{

	// This class should be access via Singleton class
	class ARENGINE_EXPORT MarkerPool
	{
		template<class MarkerPool> friend class Singleton;
	public:
		~MarkerPool();
		bool hasMarker(std::string initString);
		osg::ref_ptr<osgART::Marker> getMarker(std::string initString);

		osg::ref_ptr<osgART::TransformFilterCallback> getSmoothFilterForMarker(std::string initString);

	private:
		MarkerPool();

	private:
		osg::ref_ptr< osgART::Tracker > m_tracker;
		std::map< std::string, ref_ptr<osgART::Marker> > m_markerpool;
		std::map< std::string, ref_ptr<osgART::TransformFilterCallback> > m_smoothFilterpool;
	};

}

#endif