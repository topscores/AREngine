#ifndef __SCENE_OBJ_H__
#define __SCENE_OBJ_H__

#include "arengine/Export"
#include "arengine/NamedObj.h"

#include <osg/Node>
#include <osg/Group>
#include <osg/Vec3d>
#include <osg/MatrixTransform>

#include <string>

using namespace std;
using namespace osg;

namespace arengine
{

	enum EObjType
	{
		E_MODEL,
		E_IMAGE
	};

	class ARENGINE_EXPORT SceneObj : public osg::Group, public arengine::NamedObj
	{
	public:
		SceneObj();
		~SceneObj();

		// Override virtual function in arengine::Obj
		virtual string getName(){return getObjName();}

		virtual string getObjName() = 0;
		virtual EObjType getObjType() = 0;

		virtual ref_ptr<osg::MatrixTransform> getRotationMat();
		virtual ref_ptr<osg::MatrixTransform> getTranslationMat();
		virtual ref_ptr<osg::MatrixTransform> getScaleMat();
		virtual ref_ptr<osg::Node>			  getOrgNode();

		virtual void scale(double scale);
		virtual void scaleTo(double scale);
		virtual void translate(osg::Vec3d transVec);
		virtual void translateTo(osg::Vec3d transVec);
		virtual void rotate(osg::Vec3d rotVec);
		virtual void rotateTo(osg::Vec3d rotVec);
		virtual void appear(int markerId);
		virtual void disappear(int markerId);

	protected:
		osg::Vec3d	m_translation;
		osg::Vec3d	m_rotation;
		double		m_scale;

		osg::ref_ptr<osg::MatrixTransform> m_rotMat;
		osg::ref_ptr<osg::MatrixTransform> m_transMat;
		osg::ref_ptr<osg::MatrixTransform> m_scaleMat;

		osg::ref_ptr<osg::Node>	m_orgNode;
	};

}

#endif __SCENE_OBJ_H__