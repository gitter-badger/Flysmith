#pragma once

#define RENDERER_OGL
//#define RENDERER_DX12

// Happens with STL objects 
#pragma warning(disable:4251)

#ifdef KEBABENGINE_EXPORTS
#define KEBAB_API __declspec(dllexport)
#else
#define KEBAB_API __declspec(dllimport)
#endif

// Force the Nvidia GPU to draw Kebab(valid only when using OpenGL)
// Place before main() function.
#ifndef RENDERER_DX12
#define FORCE_NV_OPTIMUS extern "C" { __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }
#else
#define FORCE_NV_OPTIMUS
#endif

#define SAFE_RELEASE(x) if(x) \
						{ \
							(x)->Release(); \
							(x) = nullptr; \
						}

#include "Utils\Types.h"
#include "Utils\StringHash.h"