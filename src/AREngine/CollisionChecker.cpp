#include "arengine/CollisionChecker.h"
#include "arengine/Scene.h"
#include "arengine/Util.h"

#include <sstream>

using namespace std;
using namespace arengine;

CollisionChecker::CollisionChecker(DataNode *checkerNode)
:ConditionChecker(checkerNode)
{
	if (checkerNode->getNodeName().compare("Collision") == 0)
	{
		m_markerName1 = checkerNode->getAttributeAsString("markerName1");
		m_markerName2 = checkerNode->getAttributeAsString("markerName2");
		m_threshold = checkerNode->getAttributeAsInt("threshold");

		stringstream sstr;
		sstr << "marker=" << m_markerName1 << " and marker=" << m_markerName2 << " threshold=" << m_threshold;
		Util::log(__FUNCTION__, sstr.str().c_str(), 4);
	}
	else
	{
		Util::log(__FUNCTION__, "Not a Collision tag", 2);
	}
}


CollisionChecker::~CollisionChecker()
{
}


bool
CollisionChecker::conditionValid(osg::Node *node)
{
	if (!m_osgMarker1.valid() || !m_osgMarker2.valid())
	{
		ref_ptr<Scene> scene = dynamic_cast<Scene*>(node);
		if (scene.valid())
		{
			ref_ptr<Marker> marker1, marker2;

			marker1 = scene->getMarker(m_markerName1);
			marker2 = scene->getMarker(m_markerName2);

			if (!marker1.valid() || !marker2.valid())
			{
				Util::log(__FUNCTION__, "Cannot find marker", 2);
				return false;
			}
			else
			{
				m_osgMarker1 = marker1->getOsgMarker();
				m_osgMarker2 = marker2->getOsgMarker();
			}
		}
		else
		{
			Util::log(__FUNCTION__, "Invalid scene node", 2);
		}
	}

	// Determine whether both 2 markers are visible
	if (m_osgMarker1->valid() && m_osgMarker2->valid())
	{
		//get transformation marker 1
		osg::Matrix mat_marker1 = m_osgMarker1->getTransform();
		//get transformation marker 2
		osg::Matrix mat_marker2 = m_osgMarker2->getTransform();

		//get the relative position of each one
		osg::Vec3f pos1 = mat_marker1.getTrans();
		osg::Vec3f pos2 = mat_marker2.getTrans();

		//compute the euclidian distance between them
		double distance_markers12 = (pos1 - pos2).length();

		if (distance_markers12 < m_threshold)
		{
			stringstream sstr;
			sstr << "Collision detected between " << m_markerName1 << " and " << m_markerName2;
			Util::log(__FUNCTION__, sstr.str().c_str(), 5);
			return true;
		}
		else
		{
			stringstream sstr;
			sstr << "Distance between " << m_markerName1 << " and " << m_markerName2 << " = " <<  distance_markers12;
			Util::log(__FUNCTION__, sstr.str().c_str(), 5);
			return false;
		}
	}
	else
	{
		return false;
	}
}
