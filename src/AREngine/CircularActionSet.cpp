#include "arengine/CircularActionSet.h"
#include "arengine/Util.h"
#include "arengine/ActionFactory.h"

#include <sstream>
using namespace std;

using namespace arengine;

CircularActionSet::CircularActionSet(DataNode *actionSetNode)
:ActionSet(actionSetNode),
m_curAction(0)
{
}


CircularActionSet::~CircularActionSet()
{
}


void
CircularActionSet::doAction(osg::Node *node)
{
	if (m_actions.size() > 0)
	{
		m_actions[m_curAction]->exec(node);
		m_curAction = (m_curAction + 1) % m_actionCount;
	}
}


void
CircularActionSet::reset()
{
	m_curAction = 0;
}