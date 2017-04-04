/**
 * \file SumAsynchronousOperation.hpp
 * \author Ronald T. Fernandez
 * \version 1.0
 * \brief Specific version of the Asynchronous operation for the addition.
 */


#include <list>

#include "../exception/OperationNotFinishedException.hpp"
#include "AsynchronousOperation.hpp"

#ifndef SUMASYNCHRONOUSOPERATION_H_
#define SUMASYNCHRONOUSOPERATION_H_

namespace proactor {
namespace asyncOperation {

template<typename T>
class SumAsynchronousOperation : public AsynchronousOperation<T> {
private:
	std::list<T> elements;
public:
	SumAsynchronousOperation(T a) {
		elements.push_back(a);
	};
	SumAsynchronousOperation(T a, T b) {
		elements.push_back(a);
		elements.push_back(b);
	};
	SumAsynchronousOperation(std::list<T> numbers) {
		for (T num : numbers) {
			elements.push_back(num);
		}
	}
	~SumAsynchronousOperation() { elements.clear(); };

	void executeOperation() {
		AsynchronousOperation<T>::result = 0;
		for (T element: elements)
			AsynchronousOperation<T>::result += element;
		AsynchronousOperation<T>::executed = true;
	}
	T getResult() const {
		if (AsynchronousOperation<T>::executed) {
			return AsynchronousOperation<T>::result;
		}
		throw ::proactor::exception::OperationNotFinishedException();
	}
};

}
}

#endif /* SUMASYNCHRONOUSOPERATION_H_ */
