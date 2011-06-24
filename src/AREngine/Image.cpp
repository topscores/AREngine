#include "arengine/Image.h"
#include "arengine/Util.h"
#include "arengine/Singleton.h"
#include "arengine/Config.h"

#include <osgDB/ReadFile>
#include <osg/Texture2D>
#include <osg/TextureRectangle>

#include <sstream>

using namespace std;
using namespace arengine;

arengine::Image::Image(DataNode *imageNode):
m_position(0.0, 0.0), //Default position is lower left corner of screen
m_size(100.0, 100.0),  //Make a square area of 100x100 pixels
m_geode(new osg::Geode()),
m_geometry(new osg::Geometry()),
m_vertices(new osg::Vec2Array()),

//Define our shape to be a polygon
m_indices(new osg::DrawElementsUInt(osg::PrimitiveSet::POLYGON, 0)),
m_normals(new osg::Vec3Array()),
m_stateSet(new osg::StateSet()),
m_texCoord(new osg::Vec2Array()),
m_image(NULL),
m_tex(NULL)
{
	if (imageNode->getNodeName().compare("Image") == 0)
	{
		m_objName	= imageNode->getAttributeAsString("name");
		if (m_objName.empty())
		{
			char s[10];
			sprintf(s, "img%d", Util::getUniqueId());
			m_objName.append(s);
		}

		m_imageFileName = imageNode->getAttributeAsString("fileName");

		if (!m_imageFileName.empty())
		{
			m_image = osgDB::readImageFile(m_imageFileName);
			if (!m_image.valid())
			{
				stringstream sstr;
				sstr << "Image::Image() : Cannot load " << m_imageFileName;
				Util::log(sstr.str().c_str(), 2);

			}
			else
			{
				// Create a geode for this image
				m_position[0] = imageNode->getAttributeAsInt("x");
				m_position[1] = imageNode->getAttributeAsInt("y");

				m_size[0] = imageNode->getAttributeAsInt("width");
				m_size[1] = imageNode->getAttributeAsInt("height");

				m_orgNode = createGeodeForImage(m_image, m_position[0], m_position[1], m_size[0], m_size[1]);

				// Set the OpenGL States of this Geometry Node
				m_orgNode->setStateSet(m_stateSet.get());
				m_stateSet->setRenderBinDetails(150, "RenderBin");
				m_stateSet->setTextureAttributeAndModes(0, m_tex.get(), osg::StateAttribute::ON);
				m_stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
				m_stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

				this->addChild(m_orgNode);
			}
		}
		else
		{
			Util::log("Image::Image() : Image file name is missing", 2);
		}
	}
}


arengine::Image::~Image()
{
}


ref_ptr<osg::Geode>
arengine::Image::createGeodeForImage(osg::Image* image,int posx, int posy, int width, int height)
{
	if (image)
	{
		if (width>0 && height>0)
		{

			float y = 1.0;
			float x = y*((float)width/float(height));

			// set up the texture.

			Config *config = Singleton<Config>::getInstance();

			osg::Texture *texture;
			float texcoord_x;
			float texcoord_y;
			if (config->useTexture2D())
			{
				osg::Texture2D *texture2D = new osg::Texture2D;
				texture2D->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
				texture2D->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
				texture2D->setResizeNonPowerOfTwoHint(false);
				texture2D->setImage(image);

				texcoord_x = 1.0f;
				texcoord_y = 1.0f;
				texture = texture2D;
			}
			else
			{
				osg::TextureRectangle *textureRect = new osg::TextureRectangle;
				textureRect->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
				textureRect->setFilter(osg::Texture::MAG_FILTER,osg::Texture::LINEAR);
				//texture->setResizeNonPowerOfTwoHint(false);
				textureRect->setImage(image);

				texcoord_x = image->s();
				texcoord_y = image->t();
				texture = textureRect;
			}

			// set up the drawstate.
			osg::StateSet* dstate = new osg::StateSet;
			dstate->setMode(GL_CULL_FACE,osg::StateAttribute::OFF);
			dstate->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
			dstate->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);

			// set up the geoset.
			Geometry* geom = new Geometry;
			geom->setStateSet(dstate);

			Vec2Array* coords = new Vec2Array(4);
			(*coords)[0].set(posx, posy + height);
			(*coords)[1].set(posx, posy);
			(*coords)[2].set(posx + width, posy);
			(*coords)[3].set(posx + width, posy + height);
			geom->setVertexArray(coords);

			Vec2Array* tcoords = new Vec2Array(4);
			(*tcoords)[0].set(0.0f*texcoord_x,1.0f*texcoord_y);
			(*tcoords)[1].set(0.0f*texcoord_x,0.0f*texcoord_y);
			(*tcoords)[2].set(1.0f*texcoord_x,0.0f*texcoord_y);
			(*tcoords)[3].set(1.0f*texcoord_x,1.0f*texcoord_y);
			geom->setTexCoordArray(0,tcoords);

			osg::Vec4Array* colours = new osg::Vec4Array(1);
			(*colours)[0].set(1.0f,1.0f,1.0,1.0f);
			geom->setColorArray(colours);
			geom->setColorBinding(Geometry::BIND_OVERALL);

			geom->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS,0,4));

			// set up the geode.
			osg::Geode* geode = new osg::Geode;
			geode->addDrawable(geom);

			return geode;

		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}
