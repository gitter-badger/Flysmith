#pragma once
#include "Event.h"
#include <utility>


namespace cuc
{
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
}