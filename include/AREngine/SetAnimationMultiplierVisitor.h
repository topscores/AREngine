#ifndef __SET_ANIMATION_MULTIPLIER_VISITOR_H__
#define __SET_ANIMATION_MULTIPLIER_VISITOR_H__

#include "arengine/Export"

#include <osg/Geode>
#include <osg/NodeVisitor>

using namespace osg;

namespace arengine
{
	class ARENGINE_EXPORT SetAnimationMultiplierVisitor : public NodeVisitor
	{
	public:
		SetAnimationMultiplierVisitor(double multipiler);
		virtual void apply(osg::Geode &node);

	private:
		double m_multiplier;
	};
}

#endif