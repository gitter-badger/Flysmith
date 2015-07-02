#include <Windows.h>

#ifdef X86
#pragma comment(lib, "glew32s.lib")
#elif defined(X64)
#pragma comment(lib, "glew64s.lib")
#endif
#pragma comment(lib, "opengl32.lib")




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

