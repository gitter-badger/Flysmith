#include "EventManager.h"
#include "EventListener.h"
#include "Event.h"
#include <cassert>
using namespace cuc;


bool EventManager::s_bInstantiated = false;

EventManager::EventManager() 
{
	assert(!s_bInstantiated);
	s_bInstantiated = true;
}

EventManager::~EventManager()
{
	s_bInstantiated = false;
}
#include <Windows.h>
void EventManager::RegisterListener(U32 eventType, EventListener* pListener)
{
	m_listeners[eventType].push_back(pListener);
}

void EventManager::RemoveListener(EventListener* pListener)
{
	auto listenerEvents = pListener->GetEvents();
	for (auto& eventType : listenerEvents)
	{
		auto& eventListeners = m_listeners[eventType];

		for (U32 idx = 0; idx < eventListeners.size(); idx++)
		{
			if (pListener == eventListeners[idx])
			{
				eventListeners.erase(eventListeners.begin() + idx);
			}
		}
	}
}

void EventManager::PostEvent(Event event)
{
	m_eventQueues[SECONDARY].push_back(std::move(event));
}

void EventManager::TriggerEvent(Event event)
{
	for (auto& pListener : m_listeners[event.type])
		pListener->HandleEvent(event);
}

void EventManager::DispatchEvents()
{
	m_eventQueues[ACTIVE].clear();
	m_eventQueues[ACTIVE] = std::move(m_eventQueues[SECONDARY]);

	for (auto& event : m_eventQueues[ACTIVE])
	{
		for (auto& pListener : m_listeners[event.type])
			pListener->HandleEvent(event);
	}
}