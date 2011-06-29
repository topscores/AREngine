#ifndef __APPEAR_CHECKER__
#define __APPEAR_CHECKER__

#include "arengine/Export"
#include "arengine/DataNode.h"
#include "arengine/ConditionalCallback.h"
#include "arengine/Marker.h"
#include "arengine/Scene.h"

#include <osg/Node>
using namespace osg;

#include <string>
using namespace std;

namespace arengine
{
	
	class ARENGINE_EXPORT AppearChecker : public ConditionChecker
	{
	public:
		AppearChecker(DataNode *checkerNode);
		~AppearChecker();

		virtual bool conditionValid(osg::Node *node);

	private:
		string m_markerName;
		string m_objName;

		ref_ptr<Marker> m_marker;
		ref_ptr<Scene> m_scene;
	};

}

#endif