#include "Event.h"
using namespace cuc;


Event KeyUpEvent::Create(const U32& keyCode)
{
	return Event("KeyUpEvent", { keyCode });
}

Event KeyDownEvent::Create(const U32& keyCode)
{
	return Event("KeyDownEvent", { keyCode });
}

Event KeyCharEvent::Create(const U32& keyCode)
{
	return Event("KeyCharEvent", { keyCode });
}

Event::Event(const std::string& type_) 
	: type(type_)
{
}

Event::Event(const std::string& type_, const std::vector<Variant32>& attributes_)
	: type(type_)
	, attributes(attributes_)
{
}
