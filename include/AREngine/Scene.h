#ifndef __SCENE_H__
#define __SCENE_H__

#include "arengine/Export"
#include "arengine/DataNode.h"
#include "arengine/Marker.h"
#include "arengine/HUDRoot.h"
#include "arengine/PendingActionCallback.h"
#include "arengine/ConditionalCallback.h"

#include <osg/Group>
#include <osgART/Tracker>

#include <string>
#include <vector>

using namespace osg;
using namespace std;

namespace arengine
{
	class ARENGINE_EXPORT Scene : public osg::Group
	{
	public:
		Scene(DataNode *sceneNode);
		~Scene();

		int getIdxForMarkerName(string name);

		void resetCallback();

		// Activate or deactivate all markers in this scene
		void setActive(bool active);
		bool getActive();

		ref_ptr<PendingActionCallback> getPendingActionCallback();

		string getName();
		int	getMarkerCount();
		
		// Return marker idx
		int addMarker(Marker *marker);
		ref_ptr<Marker> getMarker(int idx);
		ref_ptr<Marker> getMarker(string name);

		ref_ptr<HUDRoot> getHUDRoot();

	private:
		void processMarkerData(DataNode *markerNode);

	private:
		string					m_name;
		vector< ref_ptr<Marker> > m_markers;
		bool					m_active;
		ref_ptr<HUDRoot>		m_hudRoot;

		ref_ptr<PendingActionCallback> m_pendingActionCB;
		vector< ref_ptr<ConditionalCallback> > m_conditionCallbackList;
	};
}

#endif