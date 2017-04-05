/**
 * \file InitiatorCompletion.hpp
 * \author Ronald T. Fernandez
 * \version 1.0
 * \brief Performs the Initiator and Completion tasks:
 * - Initiator: Starts the invocation of the asynchronous operation via the asynchronous operation processor
 * and defines the completion handler.
 * - Completion: Notifies about the completion of an operation.
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

template <typename T>
class InitiatorCompletion : public observer::Observer<asyncOperation::AsynchronousOperation<T> > {
private:
	completionEventQueue::CompletionEventQueue<T> *completionEventQueue;
	std::shared_ptr<asyncOperationProcessor::AsynchronousOperationProcessor<T> > asynchronousOperationProcessor;
	static unsigned int clientNum;
	proactor::Proactor<T> *proactor;
	std::future<void> t;
public:
	InitiatorCompletion() :
		completionEventQueue(new completionEventQueue::CompletionEventQueue<T>()),
		asynchronousOperationProcessor(std::shared_ptr<asyncOperationProcessor::AsynchronousOperationProcessor<T> >(new asyncOperationProcessor::AsynchronousOperationProcessor<T>(2, completionEventQueue))),
		proactor(new proactor::Proactor<T>(completionEventQueue, this))
	{
		t = std::async(std::launch::async, &proactor::Proactor<T>::exec, std::ref(proactor));
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
		delete proactor;
		delete completionEventQueue;
		logger::Logger::log("Finished InitiatorCompletion.");
	};
	void processOperation(asyncOperation::AsynchronousOperation<T> *operation) {
		logger::Logger::log("Initiating operation... ");
		asynchronousOperationProcessor->addOperation(clientNum, operation);
		++clientNum;
	};
	void notify(asyncOperation::AsynchronousOperation<T> *operation, const unsigned int id=0) {
		logger::Logger::log("Notified in Initiator/Completion - id:" +
				utils::Utils::tostr(id) +
				" - Result operation: " +
				utils::Utils::tostr(operation->getResult()));
	};
};


template <typename T>
unsigned int InitiatorCompletion<T>::clientNum = 0;

} /* namespace initiatorCompletion */
} /* namespace proactor */

#endif /* INITIATORCOMPLETION_INITIATORCOMPLETION_HPP_ */
