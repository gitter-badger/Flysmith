#pragma once
#include "PublicDef.h"
#include "Event.h"
#include "EventFactory.h"


namespace cuc
{
	struct KEBAB_API KeyboardEvent : EventFactory
	{
		enum Params 
		{ 
			U32_KEY_CODE 
		};

		enum EventIds : U32
		{
			KeyUpId   = "KeyUp"_HASH,
			KeyDownId = "KeyDown"_HASH,
			KeyCharId = "KeyChar"_HASH
		};

		static Event Create(const U32 eventId, const U32 keyCode);
	};

	struct KEBAB_API MouseEvent : EventFactory
	{
		enum Params 
		{ 
			F32_POSITION_X, 
			F32_POSITION_Y 
		};

		enum EventIds : U32
		{
			LMouseUpId   = "LMouseUp"_HASH,
			LMouseDownId = "LMouseDown"_HASH,
			RMouseUpId   = "RMouseUp"_HASH,
			RMouseDownId = "RMouseDown"_HASH
		};

		static Event Create(const U32 eventId, const F32 coordX, const F32 coordY);
	};
}


