#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "arengine/Export"
#include "arengine/Singleton.h"
#include "arengine/ConfigReader.h"
#include "arengine/DataNode.h"

#define APP_CONF(s) Config::getAppConf(s)
#define INT_CONF(s) atoi(APP_CONF(s).c_str())

#include <string>
using namespace std;

namespace arengine
{

	// This class should be access via Singleton class
	class ARENGINE_EXPORT Config
	{
	template<class Config> friend class Singleton;
	public:
		~Config();

		void readConfig(string fileName);

		string	getAVIFileName();
		string	getActiveSceneName();

		bool	smoothMotion();
		bool	flipEnable();
		bool	useTexture2D();
		bool	fullScreen();
		bool	viewStat();
		int		getCheckInterval();

		string	getAppConfig(string configName);
		static string getAppConf(string configName);

	private:
		Config();
		Config(const Config&);
		Config& operator=(const Config&);

	private:
		void initReader();

		void processModelData(DataNode *modelNode);
		void processImageData(DataNode *imageNode);
		void processSceneData(DataNode *pMarkerNode);
		void processMovieData(DataNode *movieNode);

	private:
		ConfigReader *m_reader;

		string		  m_aviFileName;
		string		  m_activeSceneName;
		bool		  m_smoothMotion;
		bool		  m_flipEnable;
		bool		  m_useTexture2D;
		bool		  m_fullScreen;
		bool		  m_viewStat;
		int			  m_checkInterval;

		DataNode	 *m_appConfig;
	};

}

#endif
