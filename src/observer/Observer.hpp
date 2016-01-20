/**
 * \file Observer.hpp
 * \author Ronald T. Fernandez
 * \version 1.0
 * \brief This is an interface to implement the observer design pattern.
 */

#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

template <typename T>
class Observer {
public:
	virtual void notify(T*, const unsigned int i=0) = 0;
	virtual ~Observer() {};
};

#endif /* OBSERVER_HPP_ */
