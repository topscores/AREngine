#include "arengine/Model.h"
#include "arengine/Util.h"
#include "arengine/Exception.h"
#include "arengine/SetAnimationMultiplierVisitor.h"

#include <osg/Matrix>
#include <osg/BoundingSphere>
#include <osgDB/ReadFile>

#include <osgUtil/GLObjectsVisitor>

#include <sstream>
using namespace std;

using namespace arengine;
using namespace osgUtil;

Model::Model(DataNode *modelNode)
{
	osg::Matrix transMat;
	osg::Matrix rotMat;
	osg::Matrix scaleMat;
	try
	{
		if (modelNode != NULL)
		{
			if (modelNode->getNodeName().compare("Model") != 0)
			{
				throw Exception("Model::Model() : Not a Model tag", 2);
			}

			if (!modelNode->getAttributeAsString("name").empty())
			{
				m_objName	= modelNode->getAttributeAsString("name");
			}
			else
			{
				throw Exception("Model::Model() : Name field cannot be empty", 2);
			}

			if (!modelNode->getAttributeAsString("fileName").empty())
			{
				m_fileName	= modelNode->getAttributeAsPath("fileName");
				m_orgNode   = osgDB::readNodeFile(m_fileName);
				m_animationMultiplier = modelNode->getAttributeAsDouble("animationMultiplier");

				if (!m_orgNode)
				{
					stringstream sstr;
					sstr << "Model::Model() : Model file not found for " << m_fileName;
					throw Exception(sstr.str().c_str(), 2);
				}

				SetSyncModeVisitor ssmv(true);
				m_orgNode->accept(ssmv);

				// If animation multiplier is assigned by user
				if (m_animationMultiplier - 0.0 > 0.00000001)
				{
					SetAnimationMultiplierVisitor samv(m_animationMultiplier);
					m_orgNode->accept(samv);
				}

				m_id		= Util::getUniqueId();

				m_orgTranslation[0] = modelNode->getAttributeAsDouble("transX");
				m_orgTranslation[1] = modelNode->getAttributeAsDouble("transY");
				m_orgTranslation[2] = modelNode->getAttributeAsDouble("transZ");
				translateTo(m_orgTranslation);

				m_orgRotation[0] = modelNode->getAttributeAsDouble("rotX");
				m_orgRotation[1] = modelNode->getAttributeAsDouble("rotY");
				m_orgRotation[2] = modelNode->getAttributeAsDouble("rotZ");
				rotateTo(m_orgRotation);

				m_orgSize = modelNode->getAttributeAsDouble("size");

				// Calculate bounding box and size
				setScaleFromSize(m_orgSize);
				scaleTo(m_scale);

				m_unitTransform = new osg::MatrixTransform();
				osg::Matrix modelTranslate;
				modelTranslate.makeTranslate(-m_bs.center());
				m_unitTransform->postMult(modelTranslate);
				m_unitTransform->addChild(m_orgNode);
				
				this->addChild(m_unitTransform);
			}
			else
			{
				throw Exception("Model::Model() : FileName field cannot be empty", 2);
			}
			//compileDisplayList();
		}
		else
		{
			throw Exception("Model::Model() : Cannot create Model object from NULL pointer", 2);
		}
	}
	catch (Exception err)
	{
		Util::log(err);
	}
}


Model::~Model()
{
}


void 
Model::compileDisplayList()
{
	GLObjectsVisitor::Mode  glvMode = GLObjectsVisitor::COMPILE_DISPLAY_LISTS|
		GLObjectsVisitor::COMPILE_STATE_ATTRIBUTES;
	GLObjectsVisitor gloV(glvMode);
	this->accept(gloV);
}


string
Model::getFileName()
{
	return m_fileName;
}

void
Model::setScaleFromSize(double size)
{
	if (m_orgNode.valid())
	{
		m_size = size;
		if (m_size == 0)
		{
			m_size = 50;
		}
		m_bs = m_orgNode->getBound();
		m_scale = m_size/m_bs.radius();
	}
}


double
Model::getSize()
{
	return m_size;
}
