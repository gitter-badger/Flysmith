#pragma once
#include "KebabEngine.h"
#include "StringHash.h"
#include "Variant.h"
#include <vector>


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
	// Individual event classes provide a creation function and an enumeration of its attributes.
	class KEBAB_API Event final
	{
	public:
		// Required to avoid 2 copies for each event submitted to the manager(one to the PostEvent() - copy needed, and one to the queues - move used instead).
		// Also factory creation functions make use of moves when returning a created event.
		Event(Event&&);
		Event& operator=(Event&&);

		Event(const U32&);
		Event(const U32&, const std::vector<Variant32>&);

		// Get an attribute without going through the attributes vector:
		// Event event;
		// event[0] is the event's first attribute
		const Variant32 operator[](const std::size_t) const;

		U32 type; 

		// Will use a pool allocator shared by all events.
		std::vector<Variant32> attributes;
	};
};
