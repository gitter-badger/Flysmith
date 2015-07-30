#include "PCH.h"
#include "FlysmithGame.h"
FORCE_NV_OPTIMUS


#ifdef RENDERER_OGL
#pragma comment(lib, "KebabOGL.lib")
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