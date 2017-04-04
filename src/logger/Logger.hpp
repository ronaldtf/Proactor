/**
 * \file Logger.hpp
 * \author Ronald T. Fernandez
 * \version 1.0
 * \brief This class implements a logging mechanism for thread, so that messages
 * are shown sequentially on the screen
 */

#ifndef LOGGER_LOGGER_HPP_
#define LOGGER_LOGGER_HPP_

#include <mutex>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>

#include "../utils/Utils.hpp"

namespace proactor {
namespace logger {

class Logger : public std::ostream {
private:
	static std::mutex m;
	Logger() {};
public:
	static void log(const std::string &message) {
		std::lock_guard<std::mutex> locker(m);
		std::cout << message << std::endl << std::flush;
	}

	static void log(const std::stringstream& message) {
		std::lock_guard<std::mutex> locker(m);
		std::cout << message.str() << std::endl << std::flush;
		m.unlock();
	}

	static void log(const std::string& message, const long long operationId, const std::thread::id threadId, const std::chrono::system_clock::time_point& time) {
		log(message + proactor::utils::Utils::tostr(operationId) +
				" \t[thread: " + proactor::utils::Utils::tostr(threadId) + "] (" +
				proactor::utils::Utils::dateToString(time) + ")");
	}
};
std::mutex Logger::m;
}
}

#endif /* LOGGER_LOGGER_HPP_ */
