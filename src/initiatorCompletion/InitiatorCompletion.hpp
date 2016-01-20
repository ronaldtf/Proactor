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

#include <thread>

#include "../asyncOperationProcessor/AsynchronousOperationProcessor.hpp"
#include "../completionEventQueue/CompletionEventQueue.hpp"
#include "../constants/Constants.hpp"
#include "../logger/Logger.hpp"
#include "../proactor/Proactor.hpp"

using namespace proactor::asyncOperationProcessor;
using namespace proactor::completionEventQueue;
using namespace proactor::logger;
using namespace proactor::proactor;

namespace proactor {
namespace initiatorCompletion {

template <typename T>
class InitiatorCompletion : public Observer<AsynchronousOperation<T> > {
private:
	CompletionEventQueue<T> *completionEventQueue;
	AsynchronousOperationProcessor<T> *asynchronousOperationProcessor;
	static unsigned int clientNum;
	Proactor<T> *proactor;
	std::future<void> t;
public:
	InitiatorCompletion() :
		completionEventQueue(new CompletionEventQueue<T>()),
		asynchronousOperationProcessor(new AsynchronousOperationProcessor<T>(2, completionEventQueue)),
		proactor(new Proactor<T>(completionEventQueue, this))
	{
		t = std::async(std::launch::async, &Proactor<T>::exec, std::ref(proactor));
	};
	virtual ~InitiatorCompletion() {
		std::this_thread::sleep_for(std::chrono::milliseconds(constants::Constants::WAIT_FINISH));
		while (completionEventQueue->size() != 0) {
			Logger::log("Waiting for last elements...: " + Logger::tostr(completionEventQueue->size()));
			std::this_thread::sleep_for(std::chrono::milliseconds(constants::Constants::SHORT_SLEEP));
			continue;
		}
		proactor->canFinish(true);
		t.wait();
		delete proactor;
		delete asynchronousOperationProcessor;
		delete completionEventQueue;
		Logger::log("Finished InitiatorCompletion.");
	};
	void processOperation(AsynchronousOperation<T> *operation) {
		Logger::log("Initiating operation... ");
		asynchronousOperationProcessor->addOperation(clientNum, operation);
		++clientNum;
	};
	void notify(AsynchronousOperation<T> *operation, const unsigned int id=0) {
		Logger::log("Notified in Initiator/Completion - id:" + Logger::tostr(id) + " - Result operation: " + Logger::tostr(operation->getResult()));
	};
};


template <typename T>
unsigned int InitiatorCompletion<T>::clientNum = 0;

} /* namespace proactor */
} /* namespace proactor */

#endif /* INITIATORCOMPLETION_INITIATORCOMPLETION_HPP_ */
