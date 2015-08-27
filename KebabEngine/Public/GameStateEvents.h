#pragma once
#include "Event.h"
#include <utility>


inline KEBAB_API
Event&& CreateTickEvent(F32 dt)
{
	Event ev;
	ev.type = "Tick"_HASH;
	ev.length = 1;
	ev.data[0] = EventData(dt);

	return std::move(ev);
}