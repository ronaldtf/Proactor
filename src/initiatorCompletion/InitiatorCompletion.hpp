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
 *
 */
template <typename T>
class InitiatorCompletion : public observer::Observer<asyncOperation::AsynchronousOperation<T> > {
private:
	std::shared_ptr<completionEventQueue::CompletionEventQueue<T> > completionEventQueue;
	std::shared_ptr<asyncOperationProcessor::AsynchronousOperationProcessor<T> > asynchronousOperationProcessor;
	std::unique_ptr<proactor::Proactor<T> > proactor;
	std::future<void> t;
public:
	InitiatorCompletion() :
		completionEventQueue(std::shared_ptr<completionEventQueue::CompletionEventQueue<T> >(new completionEventQueue::CompletionEventQueue<T>())),
		asynchronousOperationProcessor(std::make_shared<asyncOperationProcessor::AsynchronousOperationProcessor<T> >(completionEventQueue)),
		proactor(std::unique_ptr<proactor::Proactor<T> >(new proactor::Proactor<T>(completionEventQueue, this)))
	{
		t = std::async(std::launch::async, &proactor::Proactor<T>::exec, proactor.get());
	};
	virtual ~InitiatorCompletion() {
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		while (completionEventQueue->size() != 0) {
			logger::Logger::log("Waiting for last elements...: " + utils::Utils::tostr(completionEventQueue->size()));
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			continue;
		}
		proactor->canFinish(true);
		t.wait();
		logger::Logger::log("Finished InitiatorCompletion.");
	};
	void processOperation(asyncOperation::AsynchronousOperation<T> *operation) {
		logger::Logger::log("Initiating operation... ");
		asynchronousOperationProcessor->addOperation(operation);
	};
	void notify(asyncOperation::AsynchronousOperation<T> *operation, const unsigned int id=0) {
		logger::Logger::log("Notified in Initiator/Completion - id:" +
				utils::Utils::tostr(id) +
				" - Result operation: " +
				utils::Utils::tostr(operation->getResult()));
	};
};

} /* namespace initiatorCompletion */
} /* namespace proactor */

#endif /* INITIATORCOMPLETION_INITIATORCOMPLETION_HPP_ */
