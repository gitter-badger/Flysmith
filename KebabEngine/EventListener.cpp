#include "EventListener.h"
#include "EventManager.h"
using namespace cuc;


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