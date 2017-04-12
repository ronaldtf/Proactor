/**
 * @file Observer.hpp
 * @author Ronald T. Fernandez
 * @version 1.0
 * @brief This is an interface to implement the observer design pattern.
 */

#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

namespace proactor {
namespace observer {

/**
 * This class is part of the Observer design pattern. It defines the interface
 * methods for the observer.
 */
template <typename T>
class Observer {
public:
	/**
	 * Notify the observer
	 * @param[in] operation	Operation that notifies the observer
	 */
	virtual void notify(T* operation) = 0;

	/**
	 * Class destructor
	 */
	virtual ~Observer() {};
};

} /* namespace observer */
} /* namespace proactor */

#endif /* OBSERVER_HPP_ */
