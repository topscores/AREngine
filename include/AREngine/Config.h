#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "arengine/Export"
#include "arengine/ConfigReader.h"
#include "arengine/DataNode.h"
//#include "Action.h"

#include <string>
using namespace std;

namespace arengine
{

	class ARENGINE_EXPORT Config
	{
	public:
		Config();
		~Config();

		void readConfig(string fileName);

		string	getAVIFileName();
		string	getActiveSceneName();

		bool	smoothMotion();
		bool	flipEnable();
		bool	useTexture2D();
		bool	fullScreen();

	private:
		void initReader();

		void processModelData(DataNode *modelNode);
		void processImageData(DataNode *imageNode);
		void processSceneData(DataNode *pMarkerNode);
		//void processTextData(DataNode *textNode);
		//void processKeyboardHandlerData(DataNode *handlerNode);
		//void processCollisionHandlerData(DataNode *handlerNode);
		//void processAppearDisappearHandlerData(DataNode *handlerNode);

		//Action* createActionFromDataNode(DataNode *pActionNode);

	private:
		ConfigReader *m_reader;

		string		  m_aviFileName;
		string		  m_activeSceneName;
		bool		  m_smoothMotion;
		bool		  m_flipEnable;
		bool		  m_useTexture2D;
		bool		  m_fullScreen;
	};

}

#endif
