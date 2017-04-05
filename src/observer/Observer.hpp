/**
 * @file Observer.hpp
 * @author Ronald T. Fernandez
 * @version 1.0
 * @brief This is an interface to implement the observer design pattern.
 */

#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

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
	 * @param[in] opId		Operation identifier that notifies the observer (optional)
	 */
	virtual void notify(T* operation, const unsigned int opId=0) = 0;

	/**
	 * Class destructor
	 */
	virtual ~Observer() {};
};

#endif /* OBSERVER_HPP_ */
