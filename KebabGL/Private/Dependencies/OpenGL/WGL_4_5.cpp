#include "PCH.h"

#include "WGL_4_5.hpp"
#include <algorithm>
#include <vector>
#include <string.h>
#include <stddef.h>

#if defined(__APPLE__)
#include <dlfcn.h>

static void* AppleGLGetProcAddress (const char *name)
{
	static void* image = NULL;
	
	if (NULL == image)
		image = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);

	return (image ? dlsym(image, name) : NULL);
}
#endif /* __APPLE__ */

#if defined(__sgi) || defined (__sun)
#include <dlfcn.h>
#include <stdio.h>

static void* SunGetProcAddress (const GLubyte* name)
{
  static void* h = NULL;
  static void* gpa;

  if (h == NULL)
  {
    if ((h = dlopen(NULL, RTLD_LAZY | RTLD_LOCAL)) == NULL) return NULL;
    gpa = dlsym(h, "glXGetProcAddress");
  }

  if (gpa != NULL)
    return ((void*(*)(const GLubyte*))gpa)(name);
  else
    return dlsym(h, (const char*)name);
}
#endif /* __sgi || __sun */

#if defined(_WIN32)

#ifdef _MSC_VER
#pragma warning(disable: 4055)
#pragma warning(disable: 4054)
#pragma warning(disable: 4996)
#endif

static int TestPointer(const PROC pTest)
{
	ptrdiff_t iTest;
	if(!pTest) return 0;
	iTest = (ptrdiff_t)pTest;
	
	if(iTest == 1 || iTest == 2 || iTest == 3 || iTest == -1) return 0;
	
	return 1;
}

static PROC WinGetProcAddress(const char *name)
{
#pragma warning(push)
#pragma warning(disable:6387)
	HMODULE glMod = NULL;
	PROC pFunc = wglGetProcAddress((LPCSTR)name);
	if(TestPointer(pFunc))
	{
		return pFunc;
	}
	glMod = GetModuleHandleA("OpenGL32.dll");
	return (PROC)GetProcAddress(glMod, (LPCSTR)name);
#pragma warning(pop)
}
	
#define IntGetProcAddress(name) WinGetProcAddress(name)
#else
	#if defined(__APPLE__)
		#define IntGetProcAddress(name) AppleGLGetProcAddress(name)
	#else
		#if defined(__sgi) || defined(__sun)
			#define IntGetProcAddress(name) SunGetProcAddress(name)
		#else /* GLX */
		    #include <GL/glx.h>

			#define IntGetProcAddress(name) (*glXGetProcAddressARB)((const GLubyte*)name)
		#endif
	#endif
#endif

namespace wgl
{
	namespace exts
	{
		LoadTest var_ARB_multisample;
		LoadTest var_ARB_extensions_string;
		LoadTest var_ARB_pixel_format;
		LoadTest var_ARB_pixel_format_float;
		LoadTest var_ARB_framebuffer_sRGB;
		LoadTest var_ARB_create_context;
		LoadTest var_ARB_create_context_profile;
		LoadTest var_ARB_create_context_robustness;
		LoadTest var_EXT_swap_control;
		LoadTest var_EXT_pixel_format_packed_float;
		LoadTest var_EXT_create_context_es2_profile;
		LoadTest var_EXT_swap_control_tear;
		LoadTest var_NV_swap_group;
		
	} //namespace exts
	
	namespace _detail
	{
		typedef const char * (CODEGEN_FUNCPTR *PFNGETEXTENSIONSSTRINGARB)(HDC);
		PFNGETEXTENSIONSSTRINGARB GetExtensionsStringARB = 0;
		
		static int Load_ARB_extensions_string()
		{
			int numFailed = 0;
			GetExtensionsStringARB = reinterpret_cast<PFNGETEXTENSIONSSTRINGARB>(IntGetProcAddress("wglGetExtensionsStringARB"));
			if(!GetExtensionsStringARB) ++numFailed;
			return numFailed;
		}
		
		typedef BOOL (CODEGEN_FUNCPTR *PFNCHOOSEPIXELFORMATARB)(HDC, const int *, const FLOAT *, UINT, int *, UINT *);
		PFNCHOOSEPIXELFORMATARB ChoosePixelFormatARB = 0;
		typedef BOOL (CODEGEN_FUNCPTR *PFNGETPIXELFORMATATTRIBFVARB)(HDC, int, int, UINT, const int *, FLOAT *);
		PFNGETPIXELFORMATATTRIBFVARB GetPixelFormatAttribfvARB = 0;
		typedef BOOL (CODEGEN_FUNCPTR *PFNGETPIXELFORMATATTRIBIVARB)(HDC, int, int, UINT, const int *, int *);
		PFNGETPIXELFORMATATTRIBIVARB GetPixelFormatAttribivARB = 0;
		
		static int Load_ARB_pixel_format()
		{
			int numFailed = 0;
			ChoosePixelFormatARB = reinterpret_cast<PFNCHOOSEPIXELFORMATARB>(IntGetProcAddress("wglChoosePixelFormatARB"));
			if(!ChoosePixelFormatARB) ++numFailed;
			GetPixelFormatAttribfvARB = reinterpret_cast<PFNGETPIXELFORMATATTRIBFVARB>(IntGetProcAddress("wglGetPixelFormatAttribfvARB"));
			if(!GetPixelFormatAttribfvARB) ++numFailed;
			GetPixelFormatAttribivARB = reinterpret_cast<PFNGETPIXELFORMATATTRIBIVARB>(IntGetProcAddress("wglGetPixelFormatAttribivARB"));
			if(!GetPixelFormatAttribivARB) ++numFailed;
			return numFailed;
		}
		
		typedef HGLRC (CODEGEN_FUNCPTR *PFNCREATECONTEXTATTRIBSARB)(HDC, HGLRC, const int *);
		PFNCREATECONTEXTATTRIBSARB CreateContextAttribsARB = 0;
		
		static int Load_ARB_create_context()
		{
			int numFailed = 0;
			CreateContextAttribsARB = reinterpret_cast<PFNCREATECONTEXTATTRIBSARB>(IntGetProcAddress("wglCreateContextAttribsARB"));
			if(!CreateContextAttribsARB) ++numFailed;
			return numFailed;
		}
		
		typedef int (CODEGEN_FUNCPTR *PFNGETSWAPINTERVALEXT)(void);
		PFNGETSWAPINTERVALEXT GetSwapIntervalEXT = 0;
		typedef BOOL (CODEGEN_FUNCPTR *PFNSWAPINTERVALEXT)(int);
		PFNSWAPINTERVALEXT SwapIntervalEXT = 0;
		
		static int Load_EXT_swap_control()
		{
			int numFailed = 0;
			GetSwapIntervalEXT = reinterpret_cast<PFNGETSWAPINTERVALEXT>(IntGetProcAddress("wglGetSwapIntervalEXT"));
			if(!GetSwapIntervalEXT) ++numFailed;
			SwapIntervalEXT = reinterpret_cast<PFNSWAPINTERVALEXT>(IntGetProcAddress("wglSwapIntervalEXT"));
			if(!SwapIntervalEXT) ++numFailed;
			return numFailed;
		}
		
		typedef BOOL (CODEGEN_FUNCPTR *PFNBINDSWAPBARRIERNV)(GLuint, GLuint);
		PFNBINDSWAPBARRIERNV BindSwapBarrierNV = 0;
		typedef BOOL (CODEGEN_FUNCPTR *PFNJOINSWAPGROUPNV)(HDC, GLuint);
		PFNJOINSWAPGROUPNV JoinSwapGroupNV = 0;
		typedef BOOL (CODEGEN_FUNCPTR *PFNQUERYFRAMECOUNTNV)(HDC, GLuint *);
		PFNQUERYFRAMECOUNTNV QueryFrameCountNV = 0;
		typedef BOOL (CODEGEN_FUNCPTR *PFNQUERYMAXSWAPGROUPSNV)(HDC, GLuint *, GLuint *);
		PFNQUERYMAXSWAPGROUPSNV QueryMaxSwapGroupsNV = 0;
		typedef BOOL (CODEGEN_FUNCPTR *PFNQUERYSWAPGROUPNV)(HDC, GLuint *, GLuint *);
		PFNQUERYSWAPGROUPNV QuerySwapGroupNV = 0;
		typedef BOOL (CODEGEN_FUNCPTR *PFNRESETFRAMECOUNTNV)(HDC);
		PFNRESETFRAMECOUNTNV ResetFrameCountNV = 0;
		
		static int Load_NV_swap_group()
		{
			int numFailed = 0;
			BindSwapBarrierNV = reinterpret_cast<PFNBINDSWAPBARRIERNV>(IntGetProcAddress("wglBindSwapBarrierNV"));
			if(!BindSwapBarrierNV) ++numFailed;
			JoinSwapGroupNV = reinterpret_cast<PFNJOINSWAPGROUPNV>(IntGetProcAddress("wglJoinSwapGroupNV"));
			if(!JoinSwapGroupNV) ++numFailed;
			QueryFrameCountNV = reinterpret_cast<PFNQUERYFRAMECOUNTNV>(IntGetProcAddress("wglQueryFrameCountNV"));
			if(!QueryFrameCountNV) ++numFailed;
			QueryMaxSwapGroupsNV = reinterpret_cast<PFNQUERYMAXSWAPGROUPSNV>(IntGetProcAddress("wglQueryMaxSwapGroupsNV"));
			if(!QueryMaxSwapGroupsNV) ++numFailed;
			QuerySwapGroupNV = reinterpret_cast<PFNQUERYSWAPGROUPNV>(IntGetProcAddress("wglQuerySwapGroupNV"));
			if(!QuerySwapGroupNV) ++numFailed;
			ResetFrameCountNV = reinterpret_cast<PFNRESETFRAMECOUNTNV>(IntGetProcAddress("wglResetFrameCountNV"));
			if(!ResetFrameCountNV) ++numFailed;
			return numFailed;
		}
		
	} //namespace _detail
	
	namespace sys
	{
		namespace 
		{
			typedef int (*PFN_LOADEXTENSION)();
			struct MapEntry
			{
				MapEntry(const char *_extName, exts::LoadTest *_extVariable)
					: extName(_extName)
					, extVariable(_extVariable)
					, loaderFunc(0)
					{}
					
				MapEntry(const char *_extName, exts::LoadTest *_extVariable, PFN_LOADEXTENSION _loaderFunc)
					: extName(_extName)
					, extVariable(_extVariable)
					, loaderFunc(_loaderFunc)
					{}
				
				const char *extName;
				exts::LoadTest *extVariable;
				PFN_LOADEXTENSION loaderFunc;
			};
			
			struct MapCompare
			{
				MapCompare(const char *test_) : test(test_) {}
				bool operator()(const MapEntry &other) { return strcmp(test, other.extName) == 0; }
				const char *test;
			};
			
			void InitializeMappingTable(std::vector<MapEntry> &table)
			{
				table.reserve(13);
				table.push_back(MapEntry("WGL_ARB_multisample", &exts::var_ARB_multisample));
				table.push_back(MapEntry("WGL_ARB_extensions_string", &exts::var_ARB_extensions_string, _detail::Load_ARB_extensions_string));
				table.push_back(MapEntry("WGL_ARB_pixel_format", &exts::var_ARB_pixel_format, _detail::Load_ARB_pixel_format));
				table.push_back(MapEntry("WGL_ARB_pixel_format_float", &exts::var_ARB_pixel_format_float));
				table.push_back(MapEntry("WGL_ARB_framebuffer_sRGB", &exts::var_ARB_framebuffer_sRGB));
				table.push_back(MapEntry("WGL_ARB_create_context", &exts::var_ARB_create_context, _detail::Load_ARB_create_context));
				table.push_back(MapEntry("WGL_ARB_create_context_profile", &exts::var_ARB_create_context_profile));
				table.push_back(MapEntry("WGL_ARB_create_context_robustness", &exts::var_ARB_create_context_robustness));
				table.push_back(MapEntry("WGL_EXT_swap_control", &exts::var_EXT_swap_control, _detail::Load_EXT_swap_control));
				table.push_back(MapEntry("WGL_EXT_pixel_format_packed_float", &exts::var_EXT_pixel_format_packed_float));
				table.push_back(MapEntry("WGL_EXT_create_context_es2_profile", &exts::var_EXT_create_context_es2_profile));
				table.push_back(MapEntry("WGL_EXT_swap_control_tear", &exts::var_EXT_swap_control_tear));
				table.push_back(MapEntry("WGL_NV_swap_group", &exts::var_NV_swap_group, _detail::Load_NV_swap_group));
			}
			
			void ClearExtensionVars()
			{
				exts::var_ARB_multisample = exts::LoadTest();
				exts::var_ARB_extensions_string = exts::LoadTest();
				exts::var_ARB_pixel_format = exts::LoadTest();
				exts::var_ARB_pixel_format_float = exts::LoadTest();
				exts::var_ARB_framebuffer_sRGB = exts::LoadTest();
				exts::var_ARB_create_context = exts::LoadTest();
				exts::var_ARB_create_context_profile = exts::LoadTest();
				exts::var_ARB_create_context_robustness = exts::LoadTest();
				exts::var_EXT_swap_control = exts::LoadTest();
				exts::var_EXT_pixel_format_packed_float = exts::LoadTest();
				exts::var_EXT_create_context_es2_profile = exts::LoadTest();
				exts::var_EXT_swap_control_tear = exts::LoadTest();
				exts::var_NV_swap_group = exts::LoadTest();
			}
			
			void LoadExtByName(std::vector<MapEntry> &table, const char *extensionName)
			{
				std::vector<MapEntry>::iterator entry = std::find_if(table.begin(), table.end(), MapCompare(extensionName));
				
				if(entry != table.end())
				{
					if(entry->loaderFunc)
						(*entry->extVariable) = exts::LoadTest(true, entry->loaderFunc());
					else
						(*entry->extVariable) = exts::LoadTest(true, 0);
				}
			}
		} //namespace 
		
		
		namespace 
		{
			static void ProcExtsFromExtString(const char *strExtList, std::vector<MapEntry> &table)
			{
				size_t iExtListLen = strlen(strExtList);
				const char *strExtListEnd = strExtList + iExtListLen;
				const char *strCurrPos = strExtList;
				char strWorkBuff[256];
			
				while(*strCurrPos)
				{
					/*Get the extension at our position.*/
					int iStrLen = 0;
					const char *strEndStr = strchr(strCurrPos, ' ');
					int iStop = 0;
					if(strEndStr == NULL)
					{
						strEndStr = strExtListEnd;
						iStop = 1;
					}
			
					iStrLen = (int)((ptrdiff_t)strEndStr - (ptrdiff_t)strCurrPos);
			
					if(iStrLen > 255)
						return;
			
					strncpy(strWorkBuff, strCurrPos, iStrLen);
					strWorkBuff[iStrLen] = '\0';
			
					LoadExtByName(table, strWorkBuff);
			
					strCurrPos = strEndStr + 1;
					if(iStop) break;
				}
			}
			
		} //namespace 
		
		exts::LoadTest LoadFunctions(HDC hdc)
		{
			ClearExtensionVars();
			std::vector<MapEntry> table;
			InitializeMappingTable(table);
			
			_detail::GetExtensionsStringARB = reinterpret_cast<_detail::PFNGETEXTENSIONSSTRINGARB>(IntGetProcAddress("wglGetExtensionsStringARB"));
			if(!_detail::GetExtensionsStringARB) return exts::LoadTest();
			
			ProcExtsFromExtString((const char *)wgl::_detail::GetExtensionsStringARB(hdc), table);
			return exts::LoadTest(true, 0);
		}
		
	} //namespace sys
} //namespace wgl