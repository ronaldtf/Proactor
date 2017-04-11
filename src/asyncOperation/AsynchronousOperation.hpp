/**
 * @file AsynchronousOperation.hpp
 * @author Ronald T. Fernandez
 * @version 1.0
 * @brief : Operation that needs to be executed (service).
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
	observer::Observer<AsynchronousOperation<T> >* observer;
	/**
	 * Result of the operation.
	 */
	T result;

	/**
	 * Class constructor.
	 */
	AsynchronousOperation()  : opId(++operationId), startTime(), endTime(), executed(false), observer(NULL), result() {
	};

	/**
	 * This method starts the execution of an operation
	 */
	virtual void executeOperation() = 0;

public:
	/**
	 * Set an observer to the operation. The observer will be notified once the operation has been finished.
	 * @param[in] observer	Observer of the class
	 * @see Observer
	 */
	void setObserver(observer::Observer<AsynchronousOperation<T> >* observer) {
		this->observer = observer;
	};

	/**
	 * This method implements the template pattern. It gets the start and end time of the operation execution
	 * and invokes the derived "executeOperation" method from the derived class.
	 */
	void execute() {
		// Set the start time
		startTime = std::chrono::system_clock::now();
		proactor::logger::Logger::log("\tStarting operation ", opId, std::this_thread::get_id(), startTime);

		// Use the template pattern
		executeOperation();

		// Set the finish time
		endTime = std::chrono::system_clock::now();
		proactor::logger::Logger::log("\tFinished operation ", opId, std::this_thread::get_id(), startTime, endTime);
		// Notify the observer, if defined
		if (observer != NULL)
			observer->notify(this);
	};

	/**
	 * Obtain the operation identifier
	 */
	const unsigned int getId() const {
		return opId;
	};

	/**
	 * Retrieve the operation result (if exists)
	 */
	virtual T getResult() const = 0;

	/**
	 * Class destructor
	 */
	virtual ~AsynchronousOperation() {};
};

template<typename T>
unsigned long long AsynchronousOperation<T>::operationId = 0;

};

}
#endif /* ASYNCHRONOUSOPERATION_H_ */
