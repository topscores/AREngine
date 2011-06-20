#include "arengine/Singleton.h"
#include "arengine/Logger.h"
using namespace arengine;

Singleton<Logger>::InstPtr Singleton<Logger>::sm_ptr;

Logger::Logger():
m_log(NULL),	// Not yet initialized
m_logLevel(-1)	// Will not log anything
{
}


Logger::~Logger()
{
	releaseLog();
}


void
Logger::init(string fileName, int logLevel)
{
	if (logLevel >= 1 && logLevel <= 5)
	{
		m_logLevel	= logLevel;
	}
	else
	{
		// Default log level = 3
		m_logLevel = 3;
	}
	m_log		= fopen(fileName.c_str(), "w");
}


void
Logger::log(string logMsg, int logLevel)
{
	if (m_log == NULL && m_logLevel == -1) // Have not initialized yet, log to console
	{
		printf(logMsg.c_str());
	}
	else if (logLevel <= m_logLevel)
	{
		fprintf(m_log, logMsg.c_str());
	}
}


void
Logger::setLogLevel(int logLevel)
{
	if (logLevel >= 1 && logLevel <= 5)
	{
		m_logLevel = logLevel;
	}
}


int
Logger::getLogLevel()
{
	return m_logLevel;
}


void
Logger::releaseLog()
{
	if (m_log)
	{
		fclose(m_log);

		m_log		= NULL;
		m_logLevel	= -1;
	}
}
