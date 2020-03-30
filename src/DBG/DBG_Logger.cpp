/*
 * DBG_Logger.cpp
 *
 *  Created on: Mar 30, 2020
 *      Author: green
 */

#include <DBG/DBG_Logger.hpp>

static char NC[] = "\033[0m";
static char DEBUG[] = "\033[90m";
static char LOG[] = "\033[0m";
static char INFO[] = "\033[36m";
static char WARN[] = "\033[0;33m";
static char ERR[] = "\033[0;31m";

static std::mutex m_mutexCout;

namespace DBG
{

	void CLogger::Debug(const char* format, ...)
	{
		std::unique_lock<std::mutex> mlock(m_mutexCout);
		fprintf(stdout, "%s", DEBUG);
		fprintf(stdout, format);
		fprintf(stdout, "%s\n", NC);
	}

	void CLogger::Log(const char *format, ...)
	{
		std::unique_lock<std::mutex> mlock(m_mutexCout);
		fprintf(stdout, "%s", LOG);
		fprintf(stdout, format);
		fprintf(stdout, "%s\n", NC);
	}

	void CLogger::Info(const char *format, ...)
	{
		std::unique_lock<std::mutex> mlock(m_mutexCout);
		fprintf(stdout, "%s", INFO);
		fprintf(stdout, format);
		fprintf(stdout, "%s\n", NC);
	}

	void CLogger::Warning(const char *format, ...)
	{
		std::unique_lock<std::mutex> mlock(m_mutexCout);
		fprintf(stderr, "%s", WARN);
		fprintf(stderr, format);
		fprintf(stderr, "%s\n", NC);
	}

	void CLogger::Error(const char *format, ...)
	{
		std::unique_lock<std::mutex> mlock(m_mutexCout);
		fprintf(stderr, "%s", ERR);
		fprintf(stderr, format);
		fprintf(stderr, "%s\n", NC);
	}

} /* namespace DBG */
