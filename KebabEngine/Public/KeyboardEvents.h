#pragma once
#include "Event.h"


inline KEBAB_API
Event&& CreateKeyUpEvent(U32 key)
{
	Event ev;
	ev.type = "KeyUp"_HASH;
	ev.length = 1;
	ev.data[0] = EventData(key);

	return std::move(ev);
}

inline KEBAB_API
Event&& CreateKeyDownEvent(U32 key)
{
	Event ev;
	ev.type = "KeyDown"_HASH;
	ev.length = 1;
	ev.data[0] = EventData(key);

	return std::move(ev);
}

inline KEBAB_API
Event&& CreateKeyCharEvent(U32 key)
{
	Event ev;
	ev.type = "KeyChar"_HASH;
	ev.length = 1;
	ev.data[0] = EventData(key);

	return std::move(ev);
}