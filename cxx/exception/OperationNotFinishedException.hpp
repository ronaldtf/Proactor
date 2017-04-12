/**
 * @file OperationNotFinishedException.hpp
 * @author Ronald T. Fernandez
 * @version 1.0
 * @brief Exception launched when the result of an unfinished operation is tried to
 * be retrieved
 */

#include <exception>
#include <iostream>

#ifndef OPERATIONNOTFINISHEDEXCEPTION_HPP_
#define OPERATIONNOTFINISHEDEXCEPTION_HPP_

#include "../logger/Logger.hpp"

namespace proactor {
namespace exception {

/**
 * This class represents an exception which has been thrown because an
 * operation has not been finished or computed
 */
class OperationNotFinishedException : public std::exception {
private:
	std::string message;
public:
	/**
	 * Class constructor
	 */
	OperationNotFinishedException() : std::exception(), message("") {
	};

	/**
	 * Class constructor
	 * @param[in] message	Message which describes the exception. The message is shown
	 * 						in the log.
	 */
	OperationNotFinishedException(std::string message) : std::exception(), message(message) {
		logger::Logger::log(message);
	}

	/**
	 * Obtain the description of the exception (the reason why the exception
	 * has been thrown)
	 */
	const std::string getMessage() const {
		return this->message;
	}
};
}
}

#endif /* OPERATIONNOTFINISHEDEXCEPTION_HPP_ */
