/**
 * @file Proactor.hpp
 * @author Ronald T. Fernandez
 * @version 1.0
 */

#ifndef PROACTOR_PROACTOR_HPP_
#define PROACTOR_PROACTOR_HPP_

#include <condition_variable>
#include <memory>
#include <mutex>
#include "../completionEventQueue/CompletionEventQueue.hpp"
#include "../logger/Logger.hpp"
#include "../observer/Observer.hpp"
#include "../utils/Utils.hpp"

namespace proactor {
namespace proactor {

/**
 * Time to keep waiting until checking whether there are more events in the completion event
 * queue
 */
static const unsigned short WAIT_TIME = 1000;

/**
 * This is the Proactor. Its mission is dequeuing completion events and then
 * notifying it to the IniitiatorCompletion handler.
 */
template <typename T>
class Proactor {
private:
	/**
	 * Queue with the completion event queue. It is being checked whether it contains
	 * or not completed operations
	 */
	std::shared_ptr<completionEventQueue::CompletionEventQueue<T> > completionEventQueue;
	/**
	 * Observer to notify that an operation has been completed. It implements the
	 * observer design pattern
	 */
	observer::Observer<asyncOperation::AsynchronousOperation<T> > *observer;
	/**
	 * Indicates whether the process is requested to be finished. I will not be finished
	 * until there are operations in the completion event queue or waiting to be finished
	 */
	bool finish;

public:
	/**
	 * Class constructor
	 * @param[in] completionEventQueue	Completion event queue, which will contain the completed
	 * 									operations
	 * @param[in] observer				Observer of this instance. The observer will be notified
	 * 									that an operation has been completed
	 */
	Proactor(std::shared_ptr<completionEventQueue::CompletionEventQueue<T> > completionEventQueue,
			 observer::Observer<asyncOperation::AsynchronousOperation<T> > *observer) :
				 completionEventQueue(completionEventQueue),
				 observer(observer) ,
				 finish(false) {
	};

	/**
	 * Class destructor
	 */
	virtual ~Proactor() {
		logger::Logger::log("Finished Proactor.");
	};

	/**
	 * Indicate that the proactor can finish its execution thread
	 * @param[in] finish	Indicates the finishing status (i.e. whether it can be finished or not)
	 */
	void canFinish(const bool finish) {
		this->finish = finish;
	}

	/**
	 * This method checks the completion event queue until the proactor is called to be finished.
	 * In case there is a new competed operations, it notifies the observer.
	 * In case the proactor is called to be finished, it wait until there are no more processes
	 * in the completion event queue and there are no operations being waiting to be completed
	 */
	void exec() {

		// The loop terminates when the proactor is called to be finished and all the operations
		// have been processed (including the ones which were being processed)
		while(!finish || completionEventQueue->arePendingOperations()) {

			// Wait for a new completed operation
			while ((completionEventQueue->size() == 0) && (!finish) )
				std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));

			// In case there are new competed events, notify the observer
			if (completionEventQueue->size() > 0) {
				logger::Logger::log("Proactor removes  element from queue (size: " +
						utils::Utils::tostr(completionEventQueue->size()) + ")...");
				asyncOperation::AsynchronousOperation<T>* myoperation = completionEventQueue->pop();
				observer->notify(myoperation);
			}
		} // The proactor is called to be finished

		logger::Logger::log("Proactor execution finished.");
	};

};

} /* namespace proactor */
} /* namespace proactor */

#endif /* PROACTOR_PROACTOR_HPP_ */
