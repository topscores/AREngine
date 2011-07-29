#ifndef __MARKER_H__
#define __MARKER_H__

#include "arengine/Export"
#include "arengine/DataNode.h"
#include "arengine/SceneObj.h"


#include <string>
#include <vector>

#include <osg/Node>
#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/NodeCallback>

#include <osgART/Tracker>
#include <osgART/Marker>

using namespace std;
using namespace osg;

namespace arengine
{

	class ARENGINE_EXPORT Marker : public MatrixTransform
	{
	public:
		Marker(DataNode *markerNode);
		~Marker();

		int		getIdxForObjName(string name);
		
		ref_ptr<osgART::Marker> getOsgMarker();
		string	getMarkerName();

		void	setActive(bool active);

		// objName represent name field in Model tag
		int		addAssociatedObj(string objName);
		void	removeAssociatedObj(string objName);
		// remove all associate obj
		void	removeAssociatedObj();

		int		countAssociatedObj();

		string	getAssociatedObjName(int idx);
		ref_ptr<SceneObj>	getAssociatedObj(int idx);
		ref_ptr<SceneObj>	getAssociatedObj(string name);

	private:
		// Relate marker transformation matrix to what information obtained from tracker
		void	initMarkerMatrixCallback();

	private:
		string			m_markerName;
		string			m_initString;
		ref_ptr<osgART::Marker> m_osgMarker;
		vector<string>	m_associatedObjNames;
		vector<ref_ptr<SceneObj>>	m_associatedObjs;

	};

}

#endif
