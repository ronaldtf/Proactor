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
#include <ostream>
#include <sstream>
#include <string>
#include <thread>

namespace proactor {
namespace logger {

class Logger : public std::ostream {
private:
	static std::mutex m;
public:
	Logger() {};
	static void log(const std::string &message) {
		m.lock();
		std::cout << message << std::endl << std::flush;
		m.unlock();
	}
	static void log(const std::stringstream &message) {
		m.lock();
		std::cout << message.str() << std::endl << std::flush;
		m.unlock();
	}
	template<typename T>
	static std::string tostr(T t) {
		std::stringstream os;
		os << t;
		return os.str();
	}
};
std::mutex Logger::m;
}
}

#endif /* LOGGER_LOGGER_HPP_ */
