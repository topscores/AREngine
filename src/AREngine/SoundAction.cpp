#include "arengine/SoundAction.h"

using namespace arengine;

StartSound::StartSound(DataNode *soundNode)
:Action(soundNode)
{
}


StartSound::~StartSound()
{
}


void
StartSound::doAction(osg::Node *node)
{
}

//==================================
StopSound::StopSound(DataNode *soundNode)
:Action(soundNode)
{
}


StopSound::~StopSound()
{
}


void
StopSound::doAction(osg::Node *node)
{
}