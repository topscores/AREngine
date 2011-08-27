#include "arengine/Config.h"
#include "arengine/Singleton.h"
#include "arengine/SmartSingleton.h"
#include "arengine/Util.h"
#include "arengine/Model.h"
#include "arengine/Marker.h"
#include "arengine/SceneObj.h"
#include "arengine/ObjPool.h"
#include "arengine/ARScene.h"
#include "arengine/ARRoot.h"
#include "arengine/Image.h"

#include "arengine/MasterCVReader.h"
#include "arengine/DebugCodec.h"
#include "arengine/DataNode.h"

using namespace arengine;


Config::Config():
m_reader(NULL)
{
}


Config::~Config()
{
}


void
Config::readConfig(string fileName)
{
	initReader();
	try {
#ifdef __APPLE__
		fileName = Util::getNativePath(fileName);
#endif
		DataNode *rootNode = m_reader->readConfigFile(fileName);
		if (rootNode == NULL)
		{
			throw new Exception("Config::readConfig() : Invalid configuration file", 1);
		}

		int logLevel = rootNode->getAttributeAsInt("logLevel");
		if (logLevel > 0)
		{
			Util::setLogLevel(logLevel);
		}
		else
		{
			Util::setLogLevel(3);
		}

		m_fullScreen	=	rootNode->getAttributeAsBool("fullScreen");
		Util::log(__FUNCTION__, 3, "fullscreen = %s", boolToString(m_fullScreen));

		m_aviFileName	=	rootNode->getAttributeAsString("aviFileName");
		if (!m_aviFileName.empty())
		{
			Util::log(__FUNCTION__, 3, "aviFileName = %s", m_aviFileName.c_str());
		}

		m_activeSceneName =   rootNode->getAttributeAsString("activeSceneName");
		if (!m_activeSceneName.empty())
		{
			Util::log(__FUNCTION__, 3, "activeSceneName = %s", m_activeSceneName.c_str());
		}

		m_smoothMotion	=	rootNode->getAttributeAsBool("smoothMotion");
		Util::log(__FUNCTION__, 3, "smoothMotion = %s", boolToString(m_smoothMotion));

		m_flipEnable	=	rootNode->getAttributeAsBool("flipEnable");
		Util::log(__FUNCTION__, 3, "flipEnable = %s", boolToString(m_flipEnable));

		m_useTexture2D	=	rootNode->getAttributeAsBool("useTexture2D");
		Util::log(__FUNCTION__, 3, "useTexture2D = %s", boolToString(m_useTexture2D));

		m_viewStat		=	rootNode->getAttributeAsBool("viewStat");
		Util::log(__FUNCTION__, 3, "viewStat = %s", boolToString(m_viewStat));

		// How often we should check the condition
		m_checkInterval = rootNode->getAttributeAsInt("checkInterval");
		// Default interval = 400 millisec (25 times / sec)
		if (m_checkInterval == 0)
		{
			m_checkInterval = 40;
		}

		// Init ARScene, VideoBackground and Tracker
		ref_ptr<ARScene> arscene = SmartSingleton<ARScene>::getInstance();
		arscene->init();

		int tagCount;
		
		// Process SceneObject tag data 
		tagCount = rootNode->countChild("SceneObjectSet");
		for (int i = 0;i < tagCount;i++)
		{
			DataNode *sceneObjectNode = rootNode->getChild("SceneObjectSet", i);

			// Process all types of object that can be displayed on scene
			int childCount = sceneObjectNode->getChildCount();
			for (int j = 0;j < childCount;j++)
			{
				DataNode *objNode = sceneObjectNode->getChild(j);
				string objName = objNode->getNodeName();

				// Process Model tag data
				if (objName.compare("Model") == 0)
				{
					processModelData(objNode);
				}
				// Process Image tag data
				else if (objName.compare("Image") == 0)
				{
					processImageData(objNode);
				}
			}
		}

		// Process Scene tag data
		tagCount = rootNode->countChild("Scene");
		for (int i = 0;i < tagCount;i++)
		{
			DataNode *sceneNode = rootNode->getChild("Scene", i);
			processSceneData(sceneNode);

		}

		// Get Application Specific data
		m_appConfig = rootNode->getChild("AppConf");
	}
	catch (Exception err)
	{
		Util::log(err);
		exit(1);
	}
}


string
Config::getAVIFileName()
{
	return m_aviFileName;
}


string
Config::getActiveSceneName()
{
	return m_activeSceneName;
}


bool
Config::smoothMotion()
{
	return m_smoothMotion;
}


bool
Config::flipEnable()
{
	return m_flipEnable;
}


bool
Config::useTexture2D()
{
	return m_useTexture2D;
}


bool
Config::fullScreen()
{
	return m_fullScreen;
}


bool
Config::viewStat()
{
	return m_viewStat;
}


int
Config::getCheckInterval()
{
	return m_checkInterval;
}


string
Config::getAppConfig(string configName)
{
	if (m_appConfig)
	{
		DataNode *conf = m_appConfig->getChild(configName);
		if (conf)
		{
			Util::log(__FUNCTION__, 3, "configName = %s value = %s", configName.c_str(), conf->getAttributeAsString("value").c_str());
			return conf->getAttributeAsString("value");
		}
		else
		{
			return string(NULL);
		}
	}
	else
	{
		return string(NULL);
	}
}


string
Config::getAppConf(std::string configName)
{
	Config *config = Singleton<Config>::getInstance();
	return config->getAppConfig(configName);
}

void
Config::initReader()
{
	if (m_reader)
	{
		delete m_reader;
	}

	MasterCVReader *reader = new MasterCVReader();
	reader->setCodec(new DebugCodec());
	m_reader = reader;
}


void
Config::processModelData(DataNode *modelNode)
{
	if (modelNode->getNodeName().compare("Model") == 0)
	{
		ref_ptr<SceneObj> model = new Model(modelNode);
		SceneObjPool *pool = Singleton<SceneObjPool>::getInstance();
		pool->addObj(model.get());
	}
}


void
Config::processImageData(DataNode *imageNode)
{
	if (imageNode->getNodeName().compare("Image") == 0)
	{
		ref_ptr<SceneObj> image = new arengine::Image(imageNode);
		SceneObjPool *pool = Singleton<SceneObjPool>::getInstance();
		pool->addObj(image.get());
	}
}


void
Config::processSceneData(DataNode *sceneNode)
{
	if (sceneNode->getNodeName().compare("Scene") == 0)
	{
		ref_ptr<Scene> scene = new Scene(sceneNode);
		ARRoot *root = dynamic_cast<ARRoot*>(SmartSingleton<ARScene>::getInstance()->getSceneData().get());
		root->addScene(scene);
	}
}