#ifndef __COLLISION_CHECKER_H__
#define __COLLISION_CHECKER_H__

#include "arengine/Export"
#include "arengine/ConditionalCallback.h"

#include <osg/Node>
#include <osgART/Marker>

namespace arengine
{

	class ARENGINE_EXPORT CollisionChecker : public ConditionChecker
	{
	public:
		CollisionChecker(DataNode *checkerNode);
		~CollisionChecker();

		virtual bool conditionValid(osg::Node *node);

	private:
		string	m_markerName1;
		string	m_markerName2;
		double	m_threshold;

		ref_ptr<osgART::Marker> m_osgMarker1;
		ref_ptr<osgART::Marker> m_osgMarker2;
	};

}

#endif