#pragma once
#include "PCH.h"
#include "EventFactory.h"
#include "Event.h"


namespace cuc
{
	struct KEBAB_API WindowResizeEvent : EventFactory
	{
		enum Params
		{
			U32_WIDTH,
			U32_HEIGHT
		};

		static Event Create(const U32 width, const U32 height);
	};
}