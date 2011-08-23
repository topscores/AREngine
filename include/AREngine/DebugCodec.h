#ifndef __DEBUG_CODEC_H__
#define __DEBUG_CODEC_H__

#include "arengine/Export"

#include <string>
#include "arengine/Codec.h"

using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT DebugCodec :
		public Codec
	{
	public:
		DebugCodec();
		virtual ~DebugCodec();

		char getNextChar(FILE* file);
		void getNextToken(FILE* file, string& token, char& lastChar);
		bool writeDataNode(FILE* file, DataNode* data);
		
		bool blankCharacter(char c);
		bool reserveCharacter(char c);

	};

}

#endif
