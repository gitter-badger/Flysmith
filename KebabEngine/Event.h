#pragma once
#include "KebabEngine.h"
#include "StringHash.h"
#include <vector>
#include "Variant.h"


namespace cuc
{
	// Individual events are created by factories, which fill up an Event object.
	// i.e. if there are 10 events, instead of deriving 10 classes from Event, there
	// are 10 factories, each one putting together an event according to its needs.
	// The system seems shittier than the previous polymorhpism-based one, but it works, for now.

	// Usage:
	// Event event = PlayerDiedEvent::Create(attribute1, attribute2);
	// if(event.type == "PlayerDied")
	// {
	//		DoAttribute1Stuff(event.attributes[PlayerDiedEvent::ATTRIBUTE1_KEY]);
	//		DoAttribute2Stuff(event.attributes[PlayerDiedEvent::ATTRIBUTE2_KEY]);
	// }

	struct KEBAB_API Event final
	{
		Event(const U32&);
		Event(const U32&, const std::vector<Variant32>&);

		U32 type; 

		// Will use a pool allocator shared by all events.
		std::vector<Variant32> attributes;
	};

	// -----------------
	// Individual event classes provide a creation function and an enumeration of its attributes.
	// -----------------
	struct KEBAB_API KeyUpEvent
	{
		enum { KEY_CODE };
		static Event Create(const U32& keyCode);
	};

	struct KEBAB_API KeyDownEvent
	{
		enum { KEY_CODE };
		static Event Create(const U32& keyCode);
	};

	struct KEBAB_API KeyCharEvent
	{
		enum { KEY_CODE };
		static Event Create(const U32& keyCode);
	};
};
