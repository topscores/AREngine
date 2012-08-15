#include "arengine/SetAnimationMultiplierVisitor.h"

#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/ImageStream>

using namespace arengine;

SetAnimationMultiplierVisitor::SetAnimationMultiplierVisitor(double multiplier)
:m_multiplier(multiplier)
{
	setTraversalMode(TRAVERSE_ALL_CHILDREN);
}

void
SetAnimationMultiplierVisitor::apply(osg::Geode &node)
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
					imageStream->setTimeMultiplier(m_multiplier);
				}
			}
		}
	}
}