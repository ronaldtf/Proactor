/**
 * @file Client.cpp
 * @author Ronald T. Fernandez
 * @version 1.0
 * This is the main class, which executes a set of asynchronous operations.
 * @see asyncOperation/AsynchronousOperation
 * @see asyncOperation/SumAsynchronousOperation
 * @see asyncOperationProcessor/InitiatorCompletion
 * @see logger/Logger
 * @see proactor/Proactor
 */

#include <iostream>
#include <memory>

#include "asyncOperation/AsynchronousOperation.hpp"
#include "asyncOperation/SumAsynchronousOperation.hpp"
#include "asyncOperationProcessor/AsynchronousOperationProcessor.hpp"
#include "initiatorCompletion/InitiatorCompletion.hpp"
#include "logger/Logger.hpp"
#include "proactor/Proactor.hpp"

using namespace proactor::asyncOperation;

int main(int argc, char *argv[]) {

	// Create operations
	SumAsynchronousOperation<int> *op1 = new SumAsynchronousOperation<int>(10, 11);
	SumAsynchronousOperation<int> *op2 = new SumAsynchronousOperation<int>(20, 21);
	SumAsynchronousOperation<int> *op3 = new SumAsynchronousOperation<int>(30, 31);
	SumAsynchronousOperation<int> *op4 = new SumAsynchronousOperation<int>(40, 41);
	SumAsynchronousOperation<int> *op5 = new SumAsynchronousOperation<int>(50, 51);
	SumAsynchronousOperation<int> *op6 = new SumAsynchronousOperation<int>(60, 61);

	// Execute operations
	proactor::logger::Logger::log("Starting operations execution...");

	proactor::initiatorCompletion::InitiatorCompletion<int> initiator;
	initiator.processOperation(op1);
	initiator.processOperation(op2);
	initiator.processOperation(op3);
	initiator.processOperation(op4);
	initiator.processOperation(op5);
	initiator.processOperation(op6);

	proactor::logger::Logger::log("Done.");

	return 0;
}
