#ifndef __CODEC_H__
#define __CODEC_H__

#include "arengine/Export"

#include <string>
#include "arengine/DataNode.h"
using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT Codec
	{
	public:
		Codec(){};
		virtual ~Codec(){};

		virtual char getNextChar(FILE* file) = 0;
		virtual void getNextToken(FILE* file, string& token, char& lastChar) = 0;
		virtual bool writeDataNode(FILE* file, DataNode* data) = 0;

		virtual bool blankCharacter(char c) = 0;
		virtual bool reserveCharacter(char c) = 0;

		void setCodecTrace(bool trace){m_trace = trace;}
		bool codecTrace(){return m_trace;}

	protected:
		bool	m_trace;
	};

}

#endif