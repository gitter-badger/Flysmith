#include "InputEvents.h"
using namespace cuc;


Event KeyboardEvent::Create(const U32 eventId, const U32 keyCode)
{
	return Event(eventId, { keyCode });
}

Event MouseEvent::Create(const U32 eventId, const F32 positionX, const F32 positionY)
{
	return Event(eventId, { positionX, positionY });
}