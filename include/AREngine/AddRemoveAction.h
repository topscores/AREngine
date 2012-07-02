#ifndef __ADD_REMOVE_ACTION_H__
#define __ADD_REMOVE_ACTION_H__

#include "arengine/Export"
#include "arengine/Action.h"
#include "arengine/DataNode.h"
#include "arengine/Marker.h"
#include "arengine/ARRoot.h"
#include "arengine/Scene.h"

namespace arengine
{

	class ARENGINE_EXPORT AddAction : public Action
	{
	public:
		AddAction(DataNode *addNode);
		virtual void doAction(osg::Node *node);
	private:
		void doAdd(Scene *targetScene);

	private:
		string m_sceneName;
		string m_markerName;
		string m_objName;
		int	   m_layerid;
		string m_target;
		
		ref_ptr<Marker> m_marker;
	};

	
	class ARENGINE_EXPORT RemoveAction : public Action
	{
	public:
		RemoveAction(DataNode *removeNode);
		virtual void doAction(osg::Node *node);

	private:
		void doRemove(Scene *targetScene);

	private:
		string m_markerName;
		string m_objName;
		int	   m_layerid;
		string m_target;

		ref_ptr<Marker> m_marker;
	};

}

#endif