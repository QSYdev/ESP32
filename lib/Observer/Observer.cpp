#include <Observer.hpp>

Observer::Observer()
{
}

Observable::Observable()
	:mObservers()
{
}

void Observable::add(Observer* observer)
{
	mObservers.push_back(observer);
}

void Observable::remove(Observer* observer)
{
	mObservers.remove(observer);
}

void Observable::notify(const Event* event)
{
	for(Observer* observer : mObservers)
		observer->notify(event);
}