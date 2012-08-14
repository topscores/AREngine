#ifndef __MOVIE_H__
#define __MOVIE_H__

#include "arengine/Export"
#include "arengine/DataNode.h"
#include "arengine/SceneObj.h"

#include <osg/Node>
#include <osg/ImageStream>

namespace arengine{

	class ARENGINE_EXPORT MovieNode : public osg::Group
	{
	private:
		osg::ref_ptr<osg::ImageStream> mImageStream;
		osg::ref_ptr<osg::Texture> mVideoTexture;
		osg::ref_ptr<osg::Geode> mVideoGeode;

	public:
		MovieNode(string filename, bool useGLLight);

		void start();
		void stop();

		void resume();
		void pause();

		void setLoopMode(bool loop);
	};

	class ARENGINE_EXPORT Movie : public SceneObj
	{
	public:
		Movie(DataNode *movieNode);
		void setScaleFromSize(double size);

		virtual string getObjName(){return m_objName;}
		virtual EObjType getObjType(){return E_MOVIE;}

		void start();
		void stop();

		void resume();
		void pause();

	private:
		string	m_objName;
		string	m_fileName;
		double	m_size;
		bool	m_useGLLight;

		osg::Vec3d m_orgTranslation;
		osg::Vec3d m_orgRotation;
		double	   m_orgSize;
		osg::Vec3d m_orgStretch;

		bool m_loop;

		ref_ptr<MatrixTransform> m_unitTransform;
		osg::BoundingSphere m_bs;
	};

}
#endif