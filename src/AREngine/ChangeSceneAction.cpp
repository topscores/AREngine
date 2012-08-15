#include "arengine/ChangeSceneAction.h"
#include "arengine/Util.h"
#include "arengine/Action.h"
#include "arengine/ARScene.h"
#include "arengine/Scene.h"

using namespace arengine;

ChangeSceneAction::ChangeSceneAction(DataNode *changeSceneNode)
:Action(changeSceneNode)
{
	if (changeSceneNode->getNodeName() == "ChangeScene")
	{
		m_sceneName	= changeSceneNode->getAttributeAsString("sceneName");
	}
	else
	{
		Util::log(__FUNCTION__, "Not an ChangeScene tag", 2);
	}
}


void
ChangeSceneAction::doAction(osg::Node *node)
{
	ref_ptr<Scene> scene = dynamic_cast<Scene*>(node);
	if (!scene.valid())
	{
		Util::log(__FUNCTION__, "Invalid scene node", 2);
	}

	if (!m_sceneName.empty())
	{
		ref_ptr<ARScene> arscene  = SmartSingleton<ARScene>::getInstance();
		ref_ptr<ARRoot> arroot = dynamic_cast<ARRoot*>(arscene->getSceneData().get());

		arroot->setActiveScene(m_sceneName);
	}
	else
	{
		Util::log(__FUNCTION__, 3, "Cannot change to scene with empty name");
	}
}