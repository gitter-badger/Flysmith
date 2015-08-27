#include "PCH.h"
#include "EventListener.h"
#include "EventManager.h"


const std::vector<U32>& EventListener::GetEvents() const
{
	return m_events;
}

EventListener::~EventListener()
{
	g_eventManager.RemoveListener(this);
}

void EventListener::RegisterForEvent(U32 hashedEventType)
{
	g_eventManager.RegisterListener(hashedEventType, this);
}