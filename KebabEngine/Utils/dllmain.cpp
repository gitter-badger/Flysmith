#include <Windows.h>
#include "KebabEngine.h"

#ifdef RENDERER_OGL
#pragma comment(lib, "D:\\Flysmith\\Bin\\Win32Debug\\KebabOGL.lib")
#elif defined(RENDERER_DX12)
#pragma comment(lib, "D:\\Flysmith\\Bin\\Win32Debug\\KebabD3D12.lib")
#endif

BOOL APIENTRY DllMain(HMODULE,
					  DWORD ul_reason_for_call,
					  LPVOID)
{
#ifdef _DEBUG
	LoadLibrary(L"D3D12SDKLayers.dll");
#endif

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

