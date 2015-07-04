#include "Event.h"
#include "Variant.h"
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

Event::Event(Event&& other)
	: type(other.type)
	, attributes(other.attributes)
{
	other.type = 0;
	other.attributes.clear();
}

Event& Event::operator=(Event&& other)
{
	std::swap(type, other.type);
	std::swap(attributes, other.attributes);

	return *this;
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
