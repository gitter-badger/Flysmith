#include "PCH.h"
#include "FlysmithGame.h"


#pragma comment(lib, "KebabD3D12.lib")

int WINAPI WinMain(_In_ HINSTANCE hInstance,
				   _In_opt_ HINSTANCE,
				   _In_ LPSTR,
				   _In_ int)
{
	FlysmithGame game(hInstance);
	game.Run();
}