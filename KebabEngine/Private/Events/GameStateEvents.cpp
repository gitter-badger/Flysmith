#include "PCH.h"
#include "GameStateEvents.h"
using namespace cuc;


Event TickEvent::Create(const F32 dt)
{
	return Event("Tick"_HASH, { dt });
}