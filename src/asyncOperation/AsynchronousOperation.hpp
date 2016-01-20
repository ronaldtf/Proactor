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
#include <thread>
#include "../constants/Constants.hpp"
#include "../observer/Observer.hpp"
#include "../logger/Logger.hpp"

using namespace proactor::logger;

namespace proactor {
namespace asyncOperation {

template<typename T>
class AsynchronousOperation {
private:
	unsigned int id;
protected:
	static unsigned int operationId;
	std::chrono::steady_clock::time_point tstart;
	std::chrono::steady_clock::time_point tend;
	bool executed;
	Observer<AsynchronousOperation<T> > *observer;
	T result;
	AsynchronousOperation()  : id(++operationId), executed(false), observer(nullptr) {};
	virtual void executeOperation() = 0;

public:
	void setObserver(Observer<AsynchronousOperation<T> > *observer) {
		Logger::log("Adding observer... \t[operation: " + Logger::tostr(id) + "]");
		this->observer = observer;
	};

	void execute() {
		tstart = std::chrono::steady_clock::now();
		Logger::log("\tStarting operation " + Logger::tostr(id) + " \t[thread: " + Logger::tostr(std::this_thread::get_id()) + "]");

		// Use the template pattern
		executeOperation();
		const int rand_num = std::rand() % 928; // 928 as a random number as well
		std::this_thread::sleep_for(std::chrono::milliseconds(constants::Constants::DELAY + rand_num));

		tend = std::chrono::steady_clock::now();
		Logger::log("\tFinished operation " + Logger::tostr(id) + "\t(time: " + Logger::tostr(std::chrono::duration_cast<std::chrono::milliseconds>(tend-tstart).count()) + ")");

		if (observer != nullptr) {
			// Notify the observer
			observer->notify(this);
		}
	};

	unsigned int getId() {
		return id;
	};

	virtual T getResult() const = 0;
	virtual ~AsynchronousOperation() {};
};

template<typename T>
unsigned int AsynchronousOperation<T>::operationId = -1;

};

}
#endif /* ASYNCHRONOUSOPERATION_H_ */
