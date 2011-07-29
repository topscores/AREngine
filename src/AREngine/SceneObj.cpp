#include "arengine/SceneObj.h"

#define PI 3.14

using namespace osg;
using namespace arengine;

SceneObj::SceneObj()
{
	m_scale = 1.0;
	m_rotation.set(osg::Vec3d(0.0, 0.0, 0.0));
	m_translation.set(osg::Vec3d(0.0, 0.0, 0.0));
	m_orgNode = NULL;

	m_scaleMat.makeScale(m_scale, m_scale, m_scale);
	m_rotMat.makeRotate(m_rotation[0], osg::Vec3f(1, 0, 0),
						m_rotation[1], osg::Vec3f(0, 1, 0),
						m_rotation[2], osg::Vec3f(0, 0, 1));
	m_transMat.makeTranslate(m_translation);

	updateTransformMat();
}


SceneObj::~SceneObj()
{
}


osg::Vec3d
SceneObj::getRotation()
{
	return m_rotation;
}


osg::Vec3d
SceneObj::getTranslation()
{
	return m_translation;
}


double
SceneObj::getScale()
{
	return m_scale;
}


ref_ptr<osg::Node>
SceneObj::getOrgNode()
{
	return m_orgNode.get();
}


void
SceneObj::scale(double scale)
{
	scaleTo(m_scale * scale);
}


void
SceneObj::scaleTo(double scale)
{
	m_scale = scale;
	m_scaleMat.makeScale(scale, scale, scale);
	updateTransformMat();
}


void
SceneObj::translate(osg::Vec3d transVec)
{
	translateTo(m_translation + transVec);
}


void
SceneObj::translateTo(osg::Vec3d transVec)
{
	m_translation = transVec;
	m_transMat.makeTranslate(m_translation);
	updateTransformMat();
}


void
SceneObj::rotate(osg::Vec3d rotVec)
{
	rotateTo(m_rotation + rotVec);
}


void
SceneObj::rotateTo(osg::Vec3d rotVec)
{
	m_rotation = rotVec;
	m_rotMat.makeRotate(m_rotation[0], osg::Vec3f(1, 0, 0),
		m_rotation[1], osg::Vec3f(0, 1, 0),
		m_rotation[2], osg::Vec3f(0, 0, 1));
	updateTransformMat();
}


void
SceneObj::updateTransformMat()
{
	this->setMatrix(m_scaleMat*m_rotMat*m_transMat);
}