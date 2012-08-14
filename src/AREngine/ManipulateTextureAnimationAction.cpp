#include "arengine/ManipulateTextureAnimationAction.h"
#include "arengine/ObjPool.h"
#include "arengine/Scene.h"

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>

using namespace arengine;
using namespace osg;
using namespace std;

ManipulateTextureAnimationVisitor::ManipulateTextureAnimationVisitor(string manipulateType)
:NodeVisitor(NodeVisitor::UPDATE_VISITOR, NodeVisitor::TRAVERSE_ALL_CHILDREN)
,m_manipulateType(manipulateType)
{
	if (m_manipulateType != "start"
		&& m_manipulateType != "stop"
		&& m_manipulateType != "pause"
		&& m_manipulateType != "resume")
	{
		Util::log(__FUNCTION__, 2, "Unsupport manipulationType:\"%s\"", m_manipulateType.c_str());
	}
}


void
ManipulateTextureAnimationVisitor::apply(osg::Geode &node)
{
	int count=node.getNumDrawables();
	for(int i=0;i<count;i++)
	{
		ref_ptr<osg::Drawable> drawable=node.getDrawable(i);
		ref_ptr<osg::Geometry> geo=dynamic_cast<osg::Geometry*>(drawable.get());
		ref_ptr<osg::StateSet> stateSet=geo->getStateSet();

		if(stateSet.valid())
		{
			ref_ptr<osg::Texture2D> texture=static_cast<osg::Texture2D*>(stateSet->getTextureAttribute(0,osg::StateAttribute::Type::TEXTURE));
			if(texture.valid())
			{
				ref_ptr<osg::ImageStream> imageStream = dynamic_cast<osg::ImageStream*>(texture->getImage());
				if (imageStream.valid())
				{
					if (m_manipulateType == "start")
					{
						imageStream->rewind();
						imageStream->play();
					}
					else if (m_manipulateType == "stop")
					{
						imageStream->pause();
						imageStream->rewind();
					}
					else if (m_manipulateType == "pause")
					{
						imageStream->pause();
					}
					else if (m_manipulateType == "resume")
					{
						imageStream->play();
					}
				}
			}
		}
	}

	traverse(node);
}


ManipulateTextureAnimationAction::ManipulateTextureAnimationAction(arengine::DataNode *datNode)
:Action(datNode)
{
	if (datNode->getNodeName() == "ManipulateTextureAnimation")
	{
		m_objName		= datNode->getAttributeAsString("objName");
		m_manipulateType= datNode->getAttributeAsString("type");
		m_obj			= Singleton<SceneObjPool>::getInstance()->getByName(m_objName);
		if (!m_obj.valid())
		{
			Util::log(__FUNCTION__, "Target object not found", 2);
		}
	}
	else
	{
		Util::log(__FUNCTION__, "Not a ManipulateTextureAnimation tag", 2);
	}
}


void
ManipulateTextureAnimationAction::doAction(osg::Node *node)
{
	ref_ptr<Scene> scene = dynamic_cast<Scene*>(node);
	if (!scene.valid())
	{
		Util::log(__FUNCTION__, "Invalid scene node", 2);
		return;
	}

	// Manipulate animation from obj on marker
	if (m_obj.valid())
	{
		ManipulateTextureAnimationVisitor mtav(m_manipulateType);
		m_obj->accept(mtav);

		Util::log(__FUNCTION__, 3, "Manipulate texture animation type=%s objName=%s", m_manipulateType.c_str(), m_objName.c_str());
	}
}
