#ifndef __HUD_ROOT_H__
#define __HUD_ROOT_H__

#include "arengine/Export"
#include "arengine/SceneObj.h"
#include "arengine/DataNode.h"

#include <osg/Group>
#include <osg/MatrixTransform>
#include <osg/Camera>

#include <vector>
#include <string>

using namespace osg;
using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT HUDRoot : public osg::Group
	{
	public:
		HUDRoot();
		HUDRoot(DataNode *hudNode);
		~HUDRoot();

		void addHUD(string name);
		void removeHUD(string name);

		ref_ptr<osg::Node> getHUD(int id);
		ref_ptr<osg::Node> getHUD(string name);

		void resizeHUD(int xMin, int xMax, int yMin, int yMax);

	private:
		void processImageData(DataNode *node);

	private:
		vector<ref_ptr<SceneObj>>	m_hudList;
		ref_ptr<osg::Camera>		m_hudCamera;
	};

}

#endif