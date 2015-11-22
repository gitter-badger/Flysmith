#include "PCH.h"
#include "FlysmithGame.h"


#pragma comment(lib, "KebabCommon.lib")
FORCE_NV_OPTIMUS
#ifdef RENDERER_GL
#pragma comment(lib, "KebabGL.lib")
#elif defined(RENDERER_DX12)
#pragma comment(lib, "KebabD3D12.lib")
#endif

int WINAPI WinMain(_In_ HINSTANCE hInstance,
				   _In_opt_ HINSTANCE,
				   _In_ LPSTR,
				   _In_ int)
{
	FlysmithGame game(hInstance);
	game.Run();
}