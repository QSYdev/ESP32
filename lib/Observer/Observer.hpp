#pragma once
#include <List.hpp>

class Event
{

};

class Observer
{

public:
	Observer();

	virtual void notify(Event* event) = 0;

};

class Observable
{

private:
	List<Observer> mObservers;

public:
	Observable();

	void add(Observer* observer);
	void remove(Observer* observer);
	void notify(Event* event);

};