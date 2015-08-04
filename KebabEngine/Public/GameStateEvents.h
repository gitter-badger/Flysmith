#pragma once
#include "PublicDef.h"
#include "EventFactory.h"
#include "Event.h"


namespace cuc
{
	struct KEBAB_API TickEvent : EventFactory
	{
		enum Params
		{
			F32_DELTA_TIME
		};

		enum EventIds : U32
		{
			TickId = "Tick"_HASH
		};

		static Event Create(const F32 dt);
	};
}