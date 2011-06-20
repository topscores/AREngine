#include "arengine/Util.h"
#include "arengine/Debugcodec.h"

using namespace arengine;

DebugCodec::DebugCodec()
{
	m_trace = false;
}


DebugCodec::~DebugCodec()
{
}


char
DebugCodec::getNextChar(FILE* file)
{
	if (file)
	{
		char c = 0;
		fscanf(file, "%c", &c);
		if (codecTrace())
		{
			string msg;
			msg.append("next char is ");
			msg.append(1, c);
			Util::log(msg, 1);
		}
		return c;
	}
	else
	{
		Util::log("Cannot getNextChar from NULL file", 1);
		return NULL;
	}
}


void
DebugCodec::getNextToken(FILE *file, string& token, char& lastChar)
{
	lastChar = 0;
	if (file)
	{
		if (!feof(file))
		{
			// Remove blank character at front of token
			do {
				if (!feof(file))
				{
					fscanf(file, "%c", &lastChar);
				}
				else
				{
					return;
				}
			} while (blankCharacter(lastChar));
			
			if (reserveCharacter(lastChar))
			{
				return;
			}

			// Add character to token until reach end of token
			do {
				if (!feof(file))
				{
					token.append(1, lastChar);
					fscanf(file, "%c", &lastChar);
				}	
				else
				{
					return;
				}
			} while(!blankCharacter(lastChar) && !reserveCharacter(lastChar));
		}
	}
	else
	{
		Util::log("Cannot getNextToken from NULL file", 1);
	}
}


bool
DebugCodec::writeDataNode(FILE* file, DataNode* data)
{
	try
	{
		if (file)
		{
			if (data->dataNodeComplete)
			{
				if (!data->getNodeName().empty())
				{
					fprintf(file,"<");
					fprintf(file, "%s", data->getNodeName().c_str());
					int countAtrrib = data->countAttribute();
					for (int i = 0;i < countAtrrib;i++)
					{
						Attribute attrib = data->getAttribute(i);
						if (!attrib.name.empty() && !attrib.value.empty())
						{
							fprintf(file, " ");
							fprintf(file, "%s=%s", attrib.name.c_str(), attrib.value.c_str());
						}
						else
						{
							throw Exception("Either attribute name or value is empty", 3);
						}
					}
					if (data->getChildCount() == 0)
					{
						fprintf(file, "/>%c", Util::newLine());
					}
					else
					{
						fprintf(file, ">%c", Util::newLine());
						int countChild = data->getChildCount();
						for (int i = 0;i < countChild;i++)
						{
							writeDataNode(file, data->getChild(i));
						}
						fprintf(file, "</");
						fprintf(file, "%s>%c", data->getNodeName().c_str(), Util::newLine());
					}
					return true;
				}
				else
				{
					throw Exception("Tag name is NULL", 3);
				}
			}
			else
			{
				throw Exception("Tag was not match properly", 3);
			}
		}
		else
		{
			throw Exception("Output was not opened properly", 3);
		}
	}
	catch (Exception err)
	{
		Util::log(err);
		return false;
	}
}


bool
DebugCodec::blankCharacter(char c)
{
	if (c == ' ' || c== '\t' || c=='\n')
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool
DebugCodec::reserveCharacter(char c)
{
	if (c == '=' || c=='<' || c=='>' || c=='/')
	{
		return true;
	}
	else
	{
		return false;
	}
}