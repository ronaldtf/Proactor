/**
 * \file AsynchronousOperation.hpp
 * \author Ronald T. Fernandez
 * \version 1.0
 * \brief : Operation that needs to be executed (service).
 * Its execution might take long (this is not a problem as the response will be returned asynchronously
 * to the entity that asked for it). In this case, it simply notifies the observer (if present).
 */


#ifndef ASYNCHRONOUSOPERATION_H_
#define ASYNCHRONOUSOPERATION_H_

#include "AsynchronousOperation.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <thread>
#include "../constants/Constants.hpp"
#include "../observer/Observer.hpp"
#include "../logger/Logger.hpp"
#include "../utils/Utils.hpp"

namespace proactor {
namespace asyncOperation {

/**
 * This class represents a generic asynchronous operation.
 * The fact of using a template class is because the output might be of a generic type.
 */
template<typename T>
class AsynchronousOperation {
private:
	/**
	 * Operation identifier for all operations. It is incremented after each operation.
	 * The first operation id is 1
	 */
	static unsigned long long operationId;
protected:
	/**
	 * Operation identifier
	 */
	const unsigned long long opId;
	/**
	 * Start time
	 */
	std::chrono::system_clock::time_point startTime;
	/**
	 * End time
	 */
	std::chrono::system_clock::time_point endTime;
	/**
	 * Indicates whether the operation has being executed or not
	 */
	bool executed;
	/**
	 * Observer of this class. In this case, we implement the observer design pattern
	 * in order to notify that the given operation has finished its execution
	 */
	Observer<AsynchronousOperation<T> >* observer;
	/**
	 * Result of the operation.
	 */
	T result;

	/**
	 * Class constructor.
	 */
	AsynchronousOperation()  : opId(operationId++), startTime(), endTime(), executed(false), observer(NULL), result() {
	};

	/**
	 * This method starts the execution of an operation
	 */
	virtual void executeOperation() = 0;

public:
	void setObserver(Observer<AsynchronousOperation<T> >* observer) {
		proactor::logger::Logger::log("Adding observer... \t[operation: " + utils::Utils::tostr(operationId) + "]");
		this->observer = observer;
	};

	void execute() {
		startTime = std::chrono::system_clock::now();
		proactor::logger::Logger::log("\tStarting operation ", operationId, std::this_thread::get_id(), startTime);

		// Use the template pattern
		executeOperation();
		// const int rand_num = std::rand() % 928; // 928 as a random number as well
		//std::this_thread::sleep_for(std::chrono::milliseconds(proactor::constants::Constants::DELAY + rand_num));

		endTime = std::chrono::system_clock::now();
		proactor::logger::Logger::log("Finished operation ", operationId, std::this_thread::get_id(), endTime);

		if (observer != nullptr) {
			// Notify the observer
			observer->notify(this);
		}
	};

	const unsigned int getId() {
		return opId;
	};

	virtual T getResult() const = 0;
	virtual ~AsynchronousOperation() {};
};

template<typename T>
unsigned long long AsynchronousOperation<T>::operationId = -1;

};

}
#endif /* ASYNCHRONOUSOPERATION_H_ */
