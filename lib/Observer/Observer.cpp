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
	mObservers.add(observer);
}

void Observable::remove(Observer* observer)
{
	mObservers.remove(observer);
}

void Observable::notify(const Event* event)
{
	mObservers.begin();
	while (!mObservers.end())
		mObservers.next()->notify(event);
}