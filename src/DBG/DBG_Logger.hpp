/*
 * DBG_Logger.hpp
 *
 *  Created on: Mar 30, 2020
 *      Author: green
 */

#ifndef SRC_DBG_DBG_LOGGER_HPP_
#define SRC_DBG_DBG_LOGGER_HPP_

#include <mutex>

namespace DBG
{

	class CLogger
	{
		public:
			static void Debug(const char* format, ...);
			static void Log(const char* format, ...);
			static void Info(const char* format, ...);
			static void Warning(const char* format, ...);
			static void Error(const char* format, ...);
	};

} /* namespace DBG */

#endif /* SRC_DBG_DBG_LOGGER_HPP_ */
