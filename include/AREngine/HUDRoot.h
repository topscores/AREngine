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
#include <map>

using namespace osg;
using namespace std;

namespace arengine
{
	class ARENGINE_EXPORT HUDLayer : public osg::Camera
	{
	public:
		HUDLayer(int layerid);
		HUDLayer(DataNode *hudNode, int layerid);

		void addHUD(string name);
		void removeHUD();
		void removeHUD(string name);
		
		ref_ptr<osg::Node> getHUD(int id);
		ref_ptr<osg::Node> getHUD(string name);
		vector< ref_ptr<SceneObj> > getHUDList();

		void resize(int xMin, int xMax, int yMin, int yMax);

	private:
		void initLayer(int width, int height, int layerid);

	private:
		int m_width;
		int m_height;
		int m_layerid;
		vector< ref_ptr<SceneObj> >	m_hudList;
	};


	class ARENGINE_EXPORT HUDRoot : public osg::Group
	{
	public:
		HUDRoot();
		~HUDRoot();
		
		void				 addLayer(DataNode *hudNode, int layerid);
		bool				 hasLayer(int layerid);
		ref_ptr<HUDLayer>	 getLayer(int layerid);
		map<int, ref_ptr<HUDLayer> > getLayerList();

		void	addHUD(string name, int layerid);
		void	removeHUD(int layerid);
		void	removeHUD(string name, int layerid);
		
		// Get HUD from any layers
		ref_ptr<osg::Node> getHUD(string name);
		// Get HUD from specific layer
		ref_ptr<osg::Node> getHUD(string name, int layerid);

		vector< ref_ptr<SceneObj> >	getHUDList();

	private:
		map< int, ref_ptr<HUDLayer> > m_layerList;
	};

}

#endif