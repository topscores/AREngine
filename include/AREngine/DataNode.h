#ifndef __DATA_NODE_H__
#define __DATA_NODE_H__

#include "arengine/Export"

#include <vector>
#include <string>
using namespace std;

namespace arengine
{

	typedef struct tagAttribute
	{
		string name;
		string value;
	} Attribute;

	class ARENGINE_EXPORT DataNode
	{
	public:
		DataNode();
		~DataNode();

		void		setParentNode(DataNode* parent);
		DataNode*	getParentNode();
		
		void		setNodeName(string nodeName);
		string&		getNodeName();

		void		addChild(DataNode* child);
		
		int			getChildCount();
		DataNode*	getChild(int index);
		
		int			countChild(string nodeName);
		DataNode*	getChild(string nodeName, int index=0);

		void		addAttribute(Attribute attrib);
		int			countAttribute();

		// If the index specified was out of range return reference to the first attribute in list
		Attribute&	getAttribute(int index);
		string		getAttributeAsString(string name);
		string		getAttributeAsPath(string name);
		int			getAttributeAsInt(string name);
		double		getAttributeAsDouble(string name);
		bool		getAttributeAsBool(string name);

	public:
		bool	dataNodeComplete;

	private:
		DataNode*			m_parent;
		vector<Attribute>	m_attribList;
		vector<DataNode*>	m_childList;
		string				m_nodeName;

		static string		m_emptyString;

	};

}

#endif
