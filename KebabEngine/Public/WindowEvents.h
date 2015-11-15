#pragma once
#include "Event.h"


inline KEBAB_API
Event&& CreateWindowResizeEvent(U32 width, U32 height)
{
	Event ev;
	ev.type = "WindowResize"_HASH;
	ev.length = 2;
	ev.data[0] = EventData(width);
	ev.data[1] = EventData(height);

	return std::move(ev);
}

inline KEBAB_API
Event&& CreateDeviceLostEvent()
{
	Event ev;
	ev.type = "DeviceLost"_HASH;

	return std::move(ev);
}

inline KEBAB_API
Event&& CreateDeviceRecoveredEvent()
{
	Event ev;
	ev.type = "DeviceRecovered"_HASH;

	return std::move(ev);
}