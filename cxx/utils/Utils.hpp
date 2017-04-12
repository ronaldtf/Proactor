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

/**
 * This class implements generic method for different purposes
 */
class Utils {
public:

	/**
	 * Converts a generic type value to a string
	 * @param[in] t	Value to be converted into a string
	 * @return		Converted value (in string format)
	 */
	template<typename T>
	static std::string tostr(T t) {
		std::stringstream os;
		os << t;
		return os.str();
	};

	/**
	 * Convert a date/time into a string
	 * @param[in] time	Date/time to convert
	 * @return			Converted date/time (in string format) with the time difference (if defined)
	 */
	static std::string dateToString(const std::chrono::system_clock::time_point& time) {
		// Get time given the time point
		const std::time_t convTime = std::chrono::system_clock::to_time_t(time);
		// Get the local time given the converted time
		std::tm timetm = *std::localtime(&convTime);
		// Convert the time into a string
		std::stringstream oss;
		oss << std::put_time(&timetm, "%c %Z") ;
		return oss.str();
	}

	/**
	 * Convert a date into a string
	 * @param[in] start	Start time
	 * @param[in] end	Finished time (optional)
	 * @return		Converted date (in string format) with the time difference (if defined)
	 */
	static std::string dateToString(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end) {
		std::stringstream oss;
		oss << dateToString(start);
		oss << " - elapsed: " << diffTimeToString(start, end) << " ms";
		return oss.str();
	}

	/**
	 * Calculate the diff time between two time points
	 * with the difference (in milliseconds)
	 * @param[in] start		Start time
	 * @param[in] end		Finished time
	 * @return				Difference of end-start
	 */
	static int diffTimeToString(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end) {
		std::chrono::milliseconds milliStart = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch());
		std::chrono::milliseconds milliEnd   = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch());
		return milliEnd.count() - milliStart.count();
	}
};

} /* namespace utils */
} /* namespace proactor */

#endif /* SRC_UTILS_H_ */
