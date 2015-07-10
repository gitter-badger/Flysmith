#include "Event.h"
#include "Variant.h"
#include <cassert>
using namespace cuc;


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

Event::Event(const U32 type_) 
	: type(type_)
{
}

Event::Event(const U32 type_, const std::vector<Variant32>& attributes_)
	: type(type_)
	, attributes(attributes_)
{
}

const Variant32 Event::operator[](const std::size_t idx) const
{
	assert(idx >= 0);
	assert(idx < attributes.size());

	return attributes[idx];
}