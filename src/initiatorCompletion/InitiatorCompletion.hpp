/**
 * @file InitiatorCompletion.hpp
 * @author Ronald T. Fernandez
 * @version 1.0
 */

#ifndef INITIATORCOMPLETION_INITIATORCOMPLETION_HPP_
#define INITIATORCOMPLETION_INITIATORCOMPLETION_HPP_

#include <future>
#include <memory>
#include <thread>

#include "../asyncOperationProcessor/AsynchronousOperationProcessor.hpp"
#include "../completionEventQueue/CompletionEventQueue.hpp"
#include "../logger/Logger.hpp"
#include "../proactor/Proactor.hpp"
#include "../utils/Utils.hpp"

namespace proactor {
namespace initiatorCompletion {

/**
 * This class performs the Initiator and Completion tasks:
 * - Initiator: Starts the invocation of the asynchronous operation via the asynchronous operation processor
 * and defines the completion handler.
 * - Completion: Notifies about the completion of an operation.
 * @see proactor/Proactor
 * @see completionEventQueue/CompletionEventQueue
 * @see asyncOperationProcessor/AsynchronousOperationProcessor
 * @see observer/Observer
 */
template <typename T>
class InitiatorCompletion : public observer::Observer<asyncOperation::AsynchronousOperation<T> > {

private:
	/**
	 * This completion event queue contains the completed operations.
	 */
	std::shared_ptr<completionEventQueue::CompletionEventQueue<T> > completionEventQueue;
	/**
	 * This is the processor of the operations. It also fills the completion event queue when an operation
	 * is finished
	 */
	std::shared_ptr<asyncOperationProcessor::AsynchronousOperationProcessor<T> > asynchronousOperationProcessor;
	/**
	 * Checks in background the status of the completion event queue and notifies this class
	 * when an operation is completed
	 */
	std::unique_ptr<proactor::Proactor<T> > proactor;
	/**
	 * This is used only when we want to finish the system. It allows finishing correctly the initiator
	 * completion by waiting until the proactor thread finishes
	 */
	std::future<void> proactorThread;

public:
	/**
	 * Class constructor
	 */
	InitiatorCompletion() :
		completionEventQueue(std::shared_ptr<completionEventQueue::CompletionEventQueue<T> >(new completionEventQueue::CompletionEventQueue<T>())),
		asynchronousOperationProcessor(std::make_shared<asyncOperationProcessor::AsynchronousOperationProcessor<T> >(completionEventQueue)),
		proactor(std::unique_ptr<proactor::Proactor<T> >(new proactor::Proactor<T>(completionEventQueue, this)))
	{
		// Start the proactor
		proactorThread = std::async(std::launch::async, &proactor::Proactor<T>::exec, proactor.get());
	};

	/**
	 * Class destructor
	 */
	virtual ~InitiatorCompletion() {
		// Verify that all operations have been processed
		while (completionEventQueue->size() != 0) {
			logger::Logger::log("Waiting for last elements...: " + utils::Utils::tostr(completionEventQueue->size()));
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		// Tell the proactor to finish
		proactor->canFinish(true);
		// Wait until the proactor finishes
		proactorThread.wait();
		logger::Logger::log("Finished InitiatorCompletion.");
	};

	/**
	 * Add an operation to the system. It adds the operation to the AsynchronousOperationProcessor,
	 * which decides whether the operation can be processed or keeps waiting until an slot is
	 * available
	 * @param[in] operation	Operation to be processed
	 */
	void processOperation(asyncOperation::AsynchronousOperation<T> *operation) {
		logger::Logger::log("Initiating operation " + utils::Utils::tostr(operation->getId()) + "... ");
		asynchronousOperationProcessor->addOperation(operation);
	};

	/**
	 * Notify that an operation has been completed
	 * @param[in] operation	Completed operation
	 */
	void notify(asyncOperation::AsynchronousOperation<T> *operation) {
		logger::Logger::log("Notified in Initiator/Completion - id:" +
				utils::Utils::tostr(operation->getId()) +
				" - Result operation: " +
				utils::Utils::tostr(operation->getResult()));
		// NOTE: Add these lines in case you want to avoid that the client removes
		//       the operation pointers
		// Remove operation as it was finished
		// delete operation;
	};
};

} /* namespace initiatorCompletion */
} /* namespace proactor */

#endif /* INITIATORCOMPLETION_INITIATORCOMPLETION_HPP_ */
