#include "PCH.h"
#include "WindowEvents.h"
using namespace cuc;


Event WindowResizeEvent::Create(const U32 width, const U32 height)
{
	return Event("WindowResize"_HASH, { width, height });
}