#ifndef __MASTER_CV_READER_H__
#define __MASTER_CV_READER_H__

#include "arengine/Configreader.h"
#include "arengine/DataNode.h"
#include "arengine/Codec.h"

#include <string>

using namespace std;
using namespace arengine;

namespace arengine{

	enum MasterCVState
	{
		WAIT_FOR_OPEN_BRACKET,
		WAIT_FOR_DATANODE_NAME,
		WAIT_FOR_ATTRIBUTE_NAME,
		WAIT_FOR_ATTRIBUTE_VALUE,
		WAIT_FOR_CLOSE_DATANODE_NAME
	};

	class ARENGINE_EXPORT MasterCVReader :
		public ConfigReader
	{
	public:
		MasterCVReader();
		virtual ~MasterCVReader();

		void setCodec(Codec *codec);

		// Caller must take responsibility for releasing resulted DataNode*
		virtual DataNode* readConfigFile(string fileName);

	private:
		Codec* m_codec;
	};

}

#endif
