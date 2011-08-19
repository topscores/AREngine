#include "arengine/Util.h"
#include "arengine/Datanode.h"

using namespace arengine;
using namespace std;

DataNode::DataNode():
m_parent(NULL),
dataNodeComplete(false)
{
}


DataNode::~DataNode()
{
	vector<DataNode*>::iterator itr;
	for (itr = m_childList.begin();itr != m_childList.end();itr++)
	{
		DataNode* pNode = *itr;
		delete pNode;
	}
	m_childList.clear();
}


void
DataNode::setParentNode(DataNode* parent)
{
	m_parent = parent;
}


DataNode*
DataNode::getParentNode()
{
	return m_parent;
}


void
DataNode::setNodeName(string nodeName)
{
	m_nodeName = nodeName;
}


string&
DataNode::getNodeName()
{
	return m_nodeName;
}


void
DataNode::addChild(DataNode* child)
{
	if (child)
	{
		m_childList.push_back(child);
		child->setParentNode(this);
	}
	else
	{
		// Do nothing
	}
}


int
DataNode::getChildCount()
{
	return m_childList.size();
}


DataNode*
DataNode::getChild(int index)
{
	if (index >= 0 && index < getChildCount())
	{
		return m_childList[index];
	}
	else
	{
		return NULL;
	}
}


int
DataNode::countChild(string nodeName)
{
	vector<DataNode*>::iterator itr;
	int count = 0;
	for (itr=m_childList.begin();itr!=m_childList.end();itr++)
	{
		DataNode *child = *itr;
		if (!child->getNodeName().compare(nodeName))
		{
			count++;
		}
		else
		{
			// Do nothing
		}
	}
	return count;
}


DataNode*
DataNode::getChild(string nodeName, int index)
{
	DataNode* pNode;
	int curIndex=0;

	vector<DataNode*>::iterator itr;
	for (itr = m_childList.begin();itr != m_childList.end();itr++)
	{
		pNode = *itr;
		if (!pNode->getNodeName().compare(nodeName))
		{
			if (curIndex == index)
			{
				return pNode;
			}
			else
			{
				curIndex++;
			}
		}
		else
		{
			// Do nothing
		}
	}

	return NULL;
}


void
DataNode::addAttribute(Attribute attrib)
{
#ifndef WIN32
	// Convert to unix style path
	attrib.value = Util::toPosixPath(attrib.value);
#endif
	m_attribList.push_back(attrib);
}


int
DataNode::countAttribute()
{
	return m_attribList.size();
}


Attribute&
DataNode::getAttribute(int index)
{
	if (index >= 0 && index < countAttribute())
	{
		return m_attribList[index];
	}
	else
	{
		return m_attribList[0];
	}
}

string DataNode::m_emptyString;

string
DataNode::getAttributeAsString(string name)
{
	vector<Attribute>::iterator itr;
	for (itr = m_attribList.begin();itr != m_attribList.end();itr++)
	{
		Attribute attrib = *itr;
		if (attrib.name.compare(name) == 0)
		{
			return attrib.value;
		}
		else
		{
			// Do nothing
		}
	}
	return m_emptyString;
}


int
DataNode::getAttributeAsInt(string name)
{
	string s = getAttributeAsString(name);
	return Util::makeInt(s);
}


double
DataNode::getAttributeAsDouble(string name)
{
	string s = getAttributeAsString(name);
	return Util::makeDouble(s);
}


bool
DataNode::getAttributeAsBool(string name)
{
	string s = getAttributeAsString(name);
	return Util::makeBool(s);
}