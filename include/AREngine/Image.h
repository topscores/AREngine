#include "arengine/Export"
#include "arengine/SceneObj.h"
#include "arengine/DataNode.h"

#include <osg/Vec2d>
#include <osg/Geode>
#include <osg/Texture>
#include <osg/Geometry>

namespace arengine
{

	class ARENGINE_EXPORT Image : public SceneObj
	{
	public:
		Image(DataNode *imageNode);
		~Image();

		string getObjName(){return m_objName;}
		EObjType getObjType(){return E_IMAGE;}

		void setImageFileName(string fileName);
		void setPosition(double x, double y);
		void setSize(int width, int height);

		string		getImageFileName();
		osg::Vec2d  getPosition();
		osg::Vec2d	getSize();

	private:
		// Modified version of osg::createGeodeForImage, create image of specified size and move to posx, posy
		ref_ptr<osg::Geode> createGeodeForImage(osg::Image* image,int posx, int posy, int width, int height);

	private:
		string			 m_objName;
		string			 m_imageFileName;
		osg::Vec2d       m_position;  //Lower left corner index position
		osg::Vec2d       m_size;      //Length/Width of the element

		osg::ref_ptr<osg::Geode>            m_geode;
		osg::ref_ptr<osg::Geometry>         m_geometry;
		osg::ref_ptr<osg::Vec2Array>        m_vertices;
		osg::ref_ptr<osg::DrawElementsUInt> m_indices;
		osg::ref_ptr<osg::Vec3Array>        m_normals;
		osg::ref_ptr<osg::StateSet>         m_stateSet;
		osg::ref_ptr<osg::Vec2Array>		m_texCoord;
		osg::ref_ptr<osg::Image>			m_image;
		osg::ref_ptr<osg::Texture>			m_tex;
	};

}