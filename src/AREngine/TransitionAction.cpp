#include "arengine/TransitionAction.h"
#include "arengine/ObjPool.h"
#include "arengine/Util.h"
#include "arengine/Singleton.h"

using namespace arengine;

TwistIn::TwistIn(DataNode *transitionNode)
:Action(transitionNode),
m_stepCount(0)
{
	if (transitionNode->getNodeName() == "TwistIn")
	{
		string objName = transitionNode->getAttributeAsString("objName");
		m_obj = Singleton<SceneObjPool>::getInstance()->getByName(objName);

		// Read in all atrribute
		osg::Vec3d twist;
		twist[0] = transitionNode->getAttributeAsDouble("twistX");
		twist[1] = transitionNode->getAttributeAsDouble("twistY");
		twist[2] = transitionNode->getAttributeAsDouble("twistZ");
		m_orgRotation = m_obj->getRotation();
		m_startRotation = m_orgRotation - twist;

		for (int i = 0;i < 3;i++)
		{
			if (m_startRotation[i] < 0)
			{
				m_startRotation[i] = m_startRotation[i] + 2*3.14;
			}
		}

		m_startScale = transitionNode->getAttributeAsDouble("fromScale");
		m_orgScale = m_obj->getScale();
		m_startScale = m_orgScale * m_startScale;

		m_stepLast = transitionNode->getAttributeAsInt("transitionStep");
		m_twistStep[0] = twist[0]/(double)m_stepLast;
		m_twistStep[1] = twist[1]/(double)m_stepLast;
		m_twistStep[2] = twist[2]/(double)m_stepLast;
		m_scaleStep = (m_orgScale-m_startScale)/m_stepLast;

	}
	else
	{
		Util::log(__FUNCTION__, "Not a TwistIn tag", 2);
	}
}


TwistIn::~TwistIn()
{
}


void
TwistIn::exec(osg::Node *node)
{
	doPreAction(node);

	addToPendingQueue(node, this);
}


void
TwistIn::doPreAction(osg::Node *node)
{
	// Scale and Rotate to start position
	m_obj->rotateTo(m_startRotation);
	m_obj->scaleTo(m_startScale);
	m_curScale = m_startScale;

	m_stepCount = 0;
}


void
TwistIn::doAction(osg::Node *node)
{
	if (m_stepCount < m_stepLast)
	{
		m_obj->rotate(m_twistStep);

		m_curScale += m_scaleStep;
		m_obj->scaleTo(m_curScale);

		addToPendingQueue(node, this);
		m_stepCount++;
	}
	else
	{
		doPostAction(node);
	}
}


void
TwistIn::doPostAction(osg::Node *node)
{
	m_obj->rotateTo(m_orgRotation);
	m_obj->scaleTo(m_orgScale);
}