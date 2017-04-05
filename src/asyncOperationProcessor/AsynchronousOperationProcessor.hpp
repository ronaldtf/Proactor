/**
 * \file AsynchronousOperationProcessor.hpp
 * \author Ronald T. Fernandez
 * \version 1.0
 * \brief Asynchronous operation processor: Executes the asynchronous operations.
 * It generates and queues the corresponding completion events.
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
#include <unordered_map>
#include "../asyncOperation/AsynchronousOperation.hpp"
#include "../completionEventQueue/CompletionEventQueue.hpp"
#include "../logger/Logger.hpp"
#include "../observer/Observer.hpp"

namespace proactor {
namespace asyncOperationProcessor  {

template <typename T>
class AsynchronousOperationProcessor : public observer::Observer<asyncOperation::AsynchronousOperation<T> > {
private:
	size_t poolSize;
	std::mutex lock;
	std::condition_variable cv;
	bool isWaiting;
	std::unordered_map<AsynchronousOperation<T>*, const unsigned int> pool;
	completionEventQueue::CompletionEventQueue<T> *completionEventQueue;
public:
	AsynchronousOperationProcessor(size_t poolSize, completionEventQueue::CompletionEventQueue<T> *completionEventQueue) :
		poolSize(poolSize), isWaiting(false), completionEventQueue(completionEventQueue) {};

	virtual ~AsynchronousOperationProcessor() {
		while (pool.size() != 0) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		logger::Logger::log("Finished AsynchronousOperationProcessor.");
	};

	void addOperation(const unsigned int id, AsynchronousOperation<T>* operation) {
		std::unique_lock<std::mutex> locker(lock);
		if (pool.size() == poolSize) {
			isWaiting = true;
			auto cond = [](size_t a, size_t b) {return a==b;};
			logger::Logger::log("Wait...");
			if (cond(pool.size(), poolSize)) {
				cv.wait(locker);
			}
		}
		operation->setObserver(this);
		std::thread t = std::thread(&AsynchronousOperation<T>::execute, operation);
		t.detach();
		pool.insert(std::pair<AsynchronousOperation<T>*,const unsigned int>(operation, id));
	};
	void notify(AsynchronousOperation<T> *operation, const unsigned int id=0) {
		std::unique_lock<std::mutex> locker(lock);

		completionEventQueue->push(operation, pool.at(operation));
		pool.erase(operation);

		if (isWaiting) {
			logger::Logger::log("Unlocking for next...");
			isWaiting = false;
			cv.notify_one();
		}
	};
};

}
}

#endif /* ASYNCHRONOUSOPERATIONPROCESSOR_H_ */
