#include "FlysmithGame.h"
#include "Events\EventListener.h"
#include "Events\Event.h"
#include "Events\InputEvents.h"
using namespace cuc;

class Listen : public EventListener
{
public:
	Listen()
	{
		RegisterForEvent("LMouseUp"_HASH);
		RegisterForEvent("LMouseDown"_HASH);
		RegisterForEvent("RMouseUp"_HASH);
		RegisterForEvent("RMouseDown"_HASH);
	}

	void HandleEvent(const Event& event)
	{
		switch (event.type)
		{
		case "LMouseUp"_HASH:
			OutputDebugStringA("L mouse up\n");
			break;
		case "RMouseUp"_HASH:
			OutputDebugStringA("R mouse up\n");
			break;
		case "LMouseDown"_HASH:
			OutputDebugStringA("L mouse down\n");
			break;
		case "RMouseDown"_HASH:
			OutputDebugStringA("R mouse down\n");
			break;
		}
	}
};



FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
{
	static Listen ass;
}