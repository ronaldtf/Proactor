/**
 * \file OperationNotFinishedException.hpp
 * \author Ronald T. Fernandez
 * \version 1.0
 * \brief Exception launched when the result of an unfinished operation is tried to
 * be retrieved
 */

#include <exception>
#include <iostream>

#ifndef OPERATIONNOTFINISHEDEXCEPTION_HPP_
#define OPERATIONNOTFINISHEDEXCEPTION_HPP_

#include "../logger/Logger.hpp"

namespace proactor {
namespace exception {

class OperationNotFinishedException : public std::exception {
public:
	OperationNotFinishedException() : std::exception() {};
	OperationNotFinishedException(std::string message) : std::exception() {
		proactor::logger::Logger::log(message);
	}
};
}
}

#endif /* OPERATIONNOTFINISHEDEXCEPTION_HPP_ */
