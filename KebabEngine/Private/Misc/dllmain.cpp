#include "PCH.h"
#include "PublicDef.h"


FORCE_NV_OPTIMUS
#ifdef RENDERER_GL
#pragma comment(lib, "KebabGL.lib")
#elif defined(RENDERER_DX12)
#pragma comment(lib, "KebabD3D12.lib")
#endif

BOOL APIENTRY DllMain(HMODULE,
					  DWORD ul_reason_for_call,
					  LPVOID)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

