#pragma once
#include "Event.h"
#include <utility>


inline KEBAB_API
Event&& CreateLMouseUpEv(U32 positionX, U32 positionY)
{
	Event ev;
	ev.type = "LMouseUp"_HASH;
	ev.length = 2;
	ev.data[0] = EventData(positionX);
	ev.data[1] = EventData(positionY);

	return std::move(ev);
}

inline KEBAB_API
Event&& CreateRMouseUpEv(U32 positionX, U32 positionY)
{
	Event ev;
	ev.type = "RMouseUp"_HASH;
	ev.length = 2;
	ev.data[0] = EventData(positionX);
	ev.data[1] = EventData(positionY);

	return std::move(ev);
}

inline KEBAB_API
Event&& CreateLMouseDownEv(U32 positionX, U32 positionY)
{
	Event ev;
	ev.type = "LMouseDown"_HASH;
	ev.length = 2;
	ev.data[0] = EventData(positionX);
	ev.data[1] = EventData(positionY);

	return std::move(ev);
}

inline KEBAB_API
Event&& CreateRMouseDownEv(U32 positionX, U32 positionY)
{
	Event ev;
	ev.type = "RMouseDown"_HASH;
	ev.length = 2;
	ev.data[0] = EventData(positionX);
	ev.data[1] = EventData(positionY);

	return std::move(ev);
}

inline KEBAB_API
Event&& CreateMouseMoveEv(U32 positionX, U32 positionY)
{
	Event ev;
	ev.type = "MouseMove"_HASH;
	ev.length = 2;
	ev.data[0] = EventData(positionX);
	ev.data[1] = EventData(positionY);

	return std::move(ev);
}

inline KEBAB_API
Event&& CreateMouseWheelEv(U32 delta)
{
	Event ev;
	ev.type = "MouseWheel"_HASH;
	ev.length = 1;
	ev.data[0] = EventData(delta);

	return std::move(ev);
}