#pragma once
#include "PCH.h"


namespace cuc
{
	class EventListener;
	class Event;

	class EventManager
	{
	public:
		EventManager();
		~EventManager();

		void RegisterListener(U32 eventType, EventListener*);
		void RemoveListener(EventListener*);

		void PostEvent(Event);
		void TriggerEvent(Event);

		void DispatchEvents();

	private:
		enum EventQueueChannel
		{
			ACTIVE,
			SECONDARY,
			NUM_CHANNELS
		};

		std::vector<Event> m_eventQueues[NUM_CHANNELS];
		std::unordered_map<U32, std::vector<EventListener*>> m_listeners;
	
		static bool s_bInstantiated;
	};

	// The global still makes me uneasy.
	extern EventManager g_eventManager;
}