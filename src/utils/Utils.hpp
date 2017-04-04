/**
 * @file Utils.h
 * @author Ronald T. Fernandez
 * @version 1.0
 */

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_

namespace proactor{
namespace utils {

class Utils {
public:
	template<typename T>
	static std::string tostr(T t) {
		std::stringstream os;
		os << t;
		return os.str();
	};

	static std::string dateToString(const std::chrono::system_clock::time_point& t) {
		const std::time_t time = std::chrono::system_clock::to_time_t(t);
		std::tm timetm = *std::localtime(&time);
		std::string output;
		std::stringstream oss;
		oss << std::put_time(&timetm, "%c %Z") << "+" << std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count() % 1000 << std::endl;
		return oss.str();
	}
};

} /* namespace utils */
} /* namespace proactor */

#endif /* SRC_UTILS_H_ */
