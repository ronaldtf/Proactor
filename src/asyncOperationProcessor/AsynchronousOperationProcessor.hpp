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
 * Asynchronous operation processor: Executes the asynchronous operations.
 * It generates and queues the corresponding completion events.
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
	 * @param[in] poolSize				Maximum size of the queue for non-completed operations
	 * @param[in] completionEventQueue	Queue of processed and completed operations. This parameter is optional (if it
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

	void addOperation(asyncOperation::AsynchronousOperation<T>* operation) {
		std::unique_lock<std::mutex> locker(lock);
		if (pool.size() == poolSize)
			cv.wait(locker, [&]{ return pool.size() < poolSize;});

		operation->setObserver(this);
		std::thread t = std::thread(&asyncOperation::AsynchronousOperation<T>::execute, operation);
		t.detach();
		pool.push_back(operation);
	};

	void notify(asyncOperation::AsynchronousOperation<T> *operation, const unsigned int id=0) {
		std::unique_lock<std::mutex> locker(lock);

		completionEventQueue->push(operation);

		bool isFull = pool.size() == poolSize;
		typedef typename std::deque<asyncOperation::AsynchronousOperation<T>*>::iterator iterType;
		iterType iter = std::find(pool.begin(), pool.end(), operation);
		if (iter != pool.end())
			pool.erase(iter);

		if (isFull) {
			logger::Logger::log("Unlocking for next...");
			cv.notify_one();
		}
	};
};

}
}

#endif /* ASYNCHRONOUSOPERATIONPROCESSOR_H_ */
