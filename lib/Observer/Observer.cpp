#include <Observer.hpp>

Observer::Observer()
{
}

SynchronousObserver::SynchronousObserver()
{
}

AsynchronousObserver::AsynchronousObserver()
	:mEvents(), mMutex(xSemaphoreCreateMutex()), mData(xSemaphoreCreateCounting(100, 0))
{
}

void AsynchronousObserver::notify(const Event* event)
{
	Event* eventToStore = nullptr;
	switch(event->mType)
	{
		case Event::event_type::PacketReceived:
		{
			eventToStore = new PacketReceived(*reinterpret_cast<const PacketReceived*>(event));
			break;
		}
	}
	xSemaphoreTake(mMutex, portTICK_PERIOD_MS);
	{
		mEvents.add(eventToStore);
		xSemaphoreGive(mData);
	}
	xSemaphoreGive(mMutex);
}

const Event* AsynchronousObserver::getEvent()
{
	const Event* event;
	xSemaphoreTake(mData, portTICK_PERIOD_MS);
	xSemaphoreTake(mMutex, portTICK_PERIOD_MS);
	{
		event = mEvents.removeFirst();
	}
	xSemaphoreGive(mMutex);

	return event;
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