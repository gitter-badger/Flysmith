#include "FlysmithGame.h"


FORCE_NV_OPTIMUS

int WINAPI WinMain(_In_ HINSTANCE hInstance,
				   _In_opt_ HINSTANCE,
				   _In_ LPSTR,
				   _In_ int)
{
	FlysmithGame game(hInstance);
	return game.Run();
}