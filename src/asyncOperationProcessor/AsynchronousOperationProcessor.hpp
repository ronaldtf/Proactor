/**
 * @file AsynchronousOperationProcessor.hpp
 * @author Ronald T. Fernandez
 * @version 1.0
 */

#ifndef ASYNCHRONOUSOPERATIONPROCESSOR_H_
#define ASYNCHRONOUSOPERATIONPROCESSOR_H_

#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <deque>
#include "../asyncOperation/AsynchronousOperation.hpp"
#include "../completionEventQueue/CompletionEventQueue.hpp"
#include "../logger/Logger.hpp"
#include "../observer/Observer.hpp"

namespace proactor {
namespace asyncOperationProcessor  {

/**
 * This class represents the asynchronous operation processor:
 * It places asynchronous operations in the execution queue, executes the
 * operations from the queue and, once the operation has finished, it queues
 * the corresponding completion events.
 */
template <typename T>
class AsynchronousOperationProcessor : public observer::Observer<asyncOperation::AsynchronousOperation<T>> {
private:
	/**
	 * Maximum pool size
	 */
	size_t poolSize;
	/**
	 * Mutex used to control the insertion in the non-completed operations queue
	 */
	std::mutex lock;
	/**
	 * Condition variable used to control the insertion regarding the queue size
	 */
	std::condition_variable cv;
	/**
	 * Pool of non-completed operations
	 */
	std::deque<asyncOperation::AsynchronousOperation<T>*> pool;
	/**
	 * Pool of completed operations (completed)
	 */
	std::shared_ptr<completionEventQueue::CompletionEventQueue<T> > completionEventQueue;
public:
	/**
	 * Default queue size for the non-completed operations, in case it is not defined
	 */
	static const size_t DEFAULT_QUEUE_SIZE = 2;

	/*
	 * Class constructor
	 * @param[in] completionEventQueue	Queue of processed and completed operations.
	 * @param[in] poolSize				Maximum size of the queue for non-completed operations. This parameter is optional (if it
	 * 									it not defined, the DEFAULT_QUEUE_SIZE is set instead)
	 */
	AsynchronousOperationProcessor( std::shared_ptr<completionEventQueue::CompletionEventQueue<T> >& completionEventQueue,
									const size_t poolSize = DEFAULT_QUEUE_SIZE) :
										poolSize(poolSize),
										pool(std::deque<asyncOperation::AsynchronousOperation<T>*>()),
										completionEventQueue(completionEventQueue) {
	};

	/**
	 * Class destructor
	 */
	virtual ~AsynchronousOperationProcessor() {
	};

	/**
	 * Add an operation to the execution queue
	 * @param[in] operation	Operation to be added to the queue and to be executed
	 */
	void addOperation(asyncOperation::AsynchronousOperation<T>* operation) {
		// Lock the queue
		std::unique_lock<std::mutex> locker(lock);
		// Wait until there is some slot free in the execution queue
		if (pool.size() == poolSize)
			cv.wait(locker, [&]{ return pool.size() < poolSize;});

		// Set this class as the observer of the operation
		operation->setObserver(this);

		// Start the operation in a new thread
		std::thread t = std::thread(&asyncOperation::AsynchronousOperation<T>::execute, operation);
		t.detach();

		// Put the operation to the execution queue
		pool.push_back(operation);
	};

	/**
	 * Notify the class that an operation has been completed.
	 * This method is part of the observer design pattern.
	 * @param[in] operation	Completed operation
	 */
	void notify(asyncOperation::AsynchronousOperation<T> *operation) {
		// Lock the queue
		std::unique_lock<std::mutex> locker(lock);

		// Add the operation to the completion event queue
		completionEventQueue->push(operation);

		// Remove the operation from the execution pool, if it exists
		bool isFull = false;
		typedef typename std::deque<asyncOperation::AsynchronousOperation<T>*>::iterator iterType;
		iterType iter = std::find(pool.begin(), pool.end(), operation);
		if (iter != pool.end()) {
			isFull = pool.size() == poolSize;
			pool.erase(iter);
		}
		// Unlock the next waiting operation
		if (isFull)
			cv.notify_one();
	};
};

}
}

#endif /* ASYNCHRONOUSOPERATIONPROCESSOR_H_ */
