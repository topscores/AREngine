#include "arengine/Util.h"
#include "arengine/Mastercvreader.h"

using namespace arengine;

MasterCVReader::MasterCVReader():
m_codec(NULL)
{
}


MasterCVReader::~MasterCVReader()
{
	if (m_codec)
	{
		delete m_codec;
		m_codec = NULL;
	}
}


void
MasterCVReader::setCodec(Codec *codec)
{
	m_codec = codec;
}


DataNode*
MasterCVReader::readConfigFile(string fileName)
{
	DataNode* parentNode = NULL;
	DataNode* curNode    = NULL;
	FILE* conf;
	Attribute attrib;
	MasterCVState state;
	char lastCharacter;
	string token;

	state = WAIT_FOR_OPEN_BRACKET;

	try
	{
		// Check if the codec was set or not
		if (!m_codec)
		{
			throw Exception("MasterCVReader::readConfigFile() : fatal error codec was not set properly", 1);
		}
		
		// State machine implement configuration reader
		conf = fopen(fileName.c_str(), "r");
		if (conf)
		{
			while (!feof(conf))
			{
				switch(state) {
				case WAIT_FOR_OPEN_BRACKET:
					char c;
					c = 0;
					c = m_codec->getNextChar(conf);
					if (c == 0 && feof(conf) && curNode->dataNodeComplete)
					{
						return curNode;
					}
					if (m_codec->blankCharacter(c))
					{
						state = WAIT_FOR_OPEN_BRACKET;
						break;
					}

					if (c == '<')
					{
						state = WAIT_FOR_DATANODE_NAME;
					}
					else
					{
						string msg;
						msg.append("MasterCVReader::readConfigFile() : Invalid configuration file expect < instead of");
						msg.append(1, c);
						throw Exception(msg, 2);
					}
					break;

				case WAIT_FOR_DATANODE_NAME:
					lastCharacter = 0;
					token.clear();

					m_codec->getNextToken(conf, token, lastCharacter);
					if (token.empty())
					{
						if (feof(conf) && curNode->dataNodeComplete)
						{
							return curNode;
						}
						else if (lastCharacter == '/')
						{
							if (!curNode->dataNodeComplete)
							{
								state = WAIT_FOR_CLOSE_DATANODE_NAME;
							}
							else
							{
								string msg;
								msg.append("MasterCVReader::readConfigFile() : Duplicate close tag for ");
								msg.append(curNode->getNodeName());
								throw Exception(msg);
							}
						}
						else
						{
							throw Exception("MasterCVReader::readConfigFile() : Invalid DataNode Name", 2);
						}
					}
					else
					{
						if (!curNode)					// This is actually root node
						{
							curNode = new DataNode();
						}
						else
						{
							parentNode = curNode;
							curNode = new DataNode();
						}
						
						if (curNode)
						{
							if (parentNode)
							{
								parentNode->addChild(curNode);
							}
							curNode->dataNodeComplete =	false;
					
							curNode->setNodeName(token);
							if (lastCharacter == ' ')
							{
								state = WAIT_FOR_ATTRIBUTE_NAME;
							}
							else if (lastCharacter == '>')
							{
								// parentNode = curNode;
								state = WAIT_FOR_OPEN_BRACKET;
							}
							else if (lastCharacter == '/')
							{
								char c = 0;
								c = m_codec->getNextChar(conf);
								if (c == '>')
								{
									curNode->dataNodeComplete = true;
									if (parentNode)
									{
										curNode = parentNode;
										parentNode = curNode->getParentNode();
									}
									state = WAIT_FOR_OPEN_BRACKET;
								}
								else
								{
									throw Exception("MasterCVReader::readConfigFile() : Invalid configuration file expect > after /", 1);
								}
							}
							else
							{
								string msg;
								msg.append("MasterCVReader::readConfigFile() : Invalid character after");
								msg.append(token);
								throw Exception(msg, 2);
							}
						}
						else
						{
							throw Exception("MasterCVReader::readConfigFile() : Cannot allocate memory for new node", 1);
						}
					}

					break;

				case WAIT_FOR_ATTRIBUTE_NAME:
					lastCharacter = 0;
					token.clear();
					m_codec->getNextToken(conf, token, lastCharacter);
					if (token.empty())
					{
						if (lastCharacter == '=')
						{
							throw Exception("MasterCVReader::readConfigFile() : Attribute with NULL name is not allowed", 1);
						}
						else
						{
							string msg;
							msg.append("MasterCVReader::readConfigFile() : Invalid atrribute name for ");
							msg.append(curNode->getNodeName());
							throw Exception(msg, 2);
						}
					}
					else
					{
						attrib.name = token;
						state		= WAIT_FOR_ATTRIBUTE_VALUE;
					}
					break;

				case WAIT_FOR_ATTRIBUTE_VALUE:
					lastCharacter = 0;
					token.clear();
					m_codec->getNextToken(conf, token, lastCharacter);
					if (token.empty())
					{
						string msg;
						msg.append("MasterCVReader::readConfigFile() : Invalid atrribute name for ");
						msg.append(curNode->getNodeName());
						throw Exception(msg, 2);
					}
					else
					{
						attrib.value = token;
						curNode->addAttribute(attrib);
						if (lastCharacter == ' ')
						{
							state = WAIT_FOR_ATTRIBUTE_NAME;
						}
						else if (lastCharacter == '>')
						{
							// parentNode = curNode;
							state = WAIT_FOR_OPEN_BRACKET;
						}
						else if (lastCharacter == '/')
						{
							char c = 0;
							c = m_codec->getNextChar(conf);
							if (c == '>')
							{
								curNode->dataNodeComplete = true;
								if (parentNode)
								{
									curNode = parentNode;
									parentNode = curNode->getParentNode();
								}
								state = WAIT_FOR_OPEN_BRACKET;
							}
							else
							{
								throw Exception("MasterCVReader::readConfigFile() : Invalid configuration file expect > after /", 2);
							}
						}
					}
					break;

				case WAIT_FOR_CLOSE_DATANODE_NAME:
					lastCharacter = 0;
					token.clear();
					m_codec->getNextToken(conf, token, lastCharacter);
					if (!token.empty())
					{
						if (!token.compare(curNode->getNodeName()))
						{
							if (curNode->dataNodeComplete)
							{
								string msg;
								msg.append("MasterCVReader::readConfigFile() : Duplicate close tag for ");
								msg.append(curNode->getNodeName());
								throw Exception(msg);
							}
							else
							{
								curNode->dataNodeComplete = true;
								if (parentNode)
								{
									curNode = parentNode;
									parentNode = curNode->getParentNode();
								}
								state = WAIT_FOR_OPEN_BRACKET;
							}
						}
						else
						{
							string msg;
							msg.append("MasterCVReader::readConfigFile() : Mismatch close tag expect ");
							msg.append(curNode->getNodeName());
							msg.append(" found ");
							msg.append(token);
							throw Exception(msg, 2);
						}
					}
					else
					{
						throw Exception("MasterCVReader::readConfigFile() : Invalid close tag name");
					}
					break;
				}
			}
			fclose(conf);
			return curNode;
		}
		else
		{
			throw Exception("fatal error cannot open configuration file", 2);
		}
	}
	catch (Exception err)
	{
		if (err.logLevel == 1)
		{
			if (conf)
			{
				fclose(conf);
			}
		}
		Util::log(err);
		return NULL;
	}
}