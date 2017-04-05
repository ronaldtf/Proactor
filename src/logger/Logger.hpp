/**
 * @file Logger.hpp
 * @author Ronald T. Fernandez
 * @version 1.0
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

/**
 * This class implements a logging mechanism. Messages are shown sequentially on the screen
 */
class Logger{
private:
	/**
	 * Mutex used to lock the output
	 */
	static std::mutex mutex;
	/**
	 * Class constructor
	 */
	Logger() {
	};
public:
	/**
	 * Display a log message in a given output
	 * @param[in] message	Message to display
	 * @param[in] ostr		Output where the message is display, or the console output
	 * 						it the parameter is not defined
	 */
	static void log(const std::string &message, std::ostream& ostr = std::cout) {
		// Lock the output
		std::lock_guard<std::mutex> locker(mutex);
		// Put and flush the message
		ostr << message << std::endl << std::flush;
	}

	/**
	 * Display a log message in a given output
	 * @param[in] message	Message to display (as string stream)
	 * @param[in] ostr		Output where the message is display, or the console output
	 * 						it the parameter is not defined
	 */
	static void log(const std::stringstream& message, std::ostream& ostr = std::cout) {
		log(message.str(), ostr);
	}

	/**
	 * Display a message in a given output and give a set of variables
	 * @param[in] message		Message to display at the beginning of the log
	 * @param[in] operationId	Operation identifier that triggers the log generation
	 * @param[in] threadId		Thread identifier which calls to this method
	 * @param[in] time			Time to display in the log
	 */
	static void log(const std::string& message,
					const long long operationId,
					const std::thread::id threadId,
					const std::chrono::system_clock::time_point& time) {
		log(message + proactor::utils::Utils::tostr(operationId) +
				" \t[thread: " + proactor::utils::Utils::tostr(threadId) + "] (" +
				proactor::utils::Utils::dateToString(time) + ")");
	}

	/**
	 * Display a message in a given output and give a set of variables
	 * @param[in] message		Message to display at the beginning of the log
	 * @param[in] operationId	Operation identifier that triggers the log generation
	 * @param[in] threadId		Thread identifier which calls to this method
	 * @param[in] startTime		Start time to compute the difference (elapsed time)
	 * @param[in] endTime		Time to display in the log
	 */
	static void log(const std::string& message,
					const long long operationId,
					const std::thread::id threadId,
					const std::chrono::system_clock::time_point& startTime,
					const std::chrono::system_clock::time_point& endTime) {
		log(message + proactor::utils::Utils::tostr(operationId) +
				" \t[thread: " + proactor::utils::Utils::tostr(threadId) + "] (" +
				proactor::utils::Utils::dateToString(startTime, endTime) + ")");
	}
};


std::mutex Logger::mutex;
}
}

#endif /* LOGGER_LOGGER_HPP_ */
