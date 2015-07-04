#include "Event.h"
using namespace cuc;


Event KeyUpEvent::Create(const U32& keyCode)
{
	return Event("KeyUpEvent"_HASH, { keyCode });
}

Event KeyDownEvent::Create(const U32& keyCode)
{
	return Event("KeyDownEvent"_HASH, { keyCode });
}

Event KeyCharEvent::Create(const U32& keyCode)
{
	return Event("KeyCharEvent"_HASH, { keyCode });
}

Event::Event(const U32& type_) 
	: type(type_)
{
}

Event::Event(const U32& type_, const std::vector<Variant32>& attributes_)
	: type(type_)
	, attributes(attributes_)
{
}
