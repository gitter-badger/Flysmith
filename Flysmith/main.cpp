#include "FlysmithGame.h"

#ifdef FORCE_NV_OPTIMUS
FORCE_NV_OPTIMUS
#endif

int WINAPI WinMain(_In_ HINSTANCE hInstance,
				   _In_opt_ HINSTANCE,
				   _In_ LPSTR,
				   _In_ int)
{
	FlysmithGame game(hInstance);
	game.Run();
}