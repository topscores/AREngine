#include "arengine/SceneObj.h"

#define PI 3.14

using namespace osg;
using namespace arengine;

SceneObj::SceneObj()
{
	m_rotation.set(osg::Vec3d(0.0, 0.0, 0.0));
	m_translation.set(osg::Vec3d(0.0, 0.0, 0.0));
	m_scale = 1.0;

	m_rotMat = new osg::MatrixTransform();
	rotateTo(m_rotation);

	m_transMat = new osg::MatrixTransform();
	translateTo(m_translation);

	m_scaleMat = new osg::MatrixTransform();
	scaleTo(m_scale);

	m_scaleMat->addChild(m_orgNode);
	m_transMat->addChild(m_scaleMat);
	m_rotMat->addChild(m_transMat);
	
	addChild(m_rotMat);
}


SceneObj::~SceneObj()
{
}


ref_ptr<osg::MatrixTransform>
SceneObj::getRotationMat()
{
	return m_rotMat.get();
}


ref_ptr<osg::MatrixTransform>
SceneObj::getTranslationMat()
{
	return m_transMat.get();
}


ref_ptr<osg::MatrixTransform>
SceneObj::getScaleMat()
{
	return m_scaleMat.get();
}


ref_ptr<osg::Node>
SceneObj::getOrgNode()
{
	return m_orgNode.get();
}


void
SceneObj::scale(double scale)
{
	m_scale = m_scale * scale;
	scaleTo(m_scale);
}


void
SceneObj::scaleTo(double scale)
{
	osg::Matrix mat;
	m_scale = scale;
	mat.makeScale(scale, scale, scale);
	m_scaleMat->setMatrix(mat);
}


void
SceneObj::translate(osg::Vec3d transVec)
{
	m_translation = m_translation + transVec;
	translateTo(m_translation);
}


void
SceneObj::translateTo(osg::Vec3d transVec)
{
	osg::Matrix mat;
	m_translation = transVec;
	mat.makeTranslate(m_translation);
	m_transMat->setMatrix(mat);
}

void
SceneObj::rotate(osg::Vec3d rotVec)
{
	m_rotation = m_rotation + rotVec;
	rotateTo(m_rotation);
}


void
SceneObj::rotateTo(osg::Vec3d rotVec)
{
	osg::Matrix mat;
	m_rotation = rotVec;
	mat.makeRotate(m_rotation[0]*PI/180, osg::Vec3f(1, 0, 0),
				   m_rotation[1]*PI/180, osg::Vec3f(0, 1, 0),
				   m_rotation[2]*PI/180, osg::Vec3f(0, 0, 1));
	m_rotMat->setMatrix(mat);
}


void
SceneObj::appear(int markerId)
{
	//MarkerManager *pMarkerManager = (MarkerManager*) ModuleRegistry::getSingleton()->getEntry("MarkerManager");
	//Marker *pMarker = pMarkerManager->getMarkerById(markerId);
	//if (pMarker)
	//{
	//	pMarker->addAssociatedModel(getName());
	//}
}


void
SceneObj::disappear(int markerId)
{
	//MarkerManager *pMarkerManager = (MarkerManager*) ModuleRegistry::getSingleton()->getEntry("MarkerManager");
	//Marker *pMarker = pMarkerManager->getMarkerById(markerId);
	//if (pMarker)
	//{
	//	pMarker->removeAssociatedModel(getName());
	//}
}