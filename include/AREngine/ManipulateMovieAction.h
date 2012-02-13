#ifndef __MANIPULATE_MOVIE_ACTION_H__
#define __MANIPULATE_MOVIE_ACTION_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"
#include "arengine/Marker.h"

#include <osg/Geode>
#include <osg/Node>

using namespace osg;

namespace arengine
{

	class ARENGINE_EXPORT ManipulateMovieAction : public Action
	{
	public:
		ManipulateMovieAction(DataNode *datNode);
		virtual void doAction(osg::Node *node);

	private:
		void manipulateAnim(Node *node, SceneObj *obj);

	private:
		string m_manipulateType;
		string m_markerName;
		string m_objName;

		ref_ptr<Marker> m_marker;
	};

}

#endif