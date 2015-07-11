#pragma once

//#define RENDERER_OGL
#define RENDERER_DX12

// Happens with STL objects 
#pragma warning(disable:4251)

#ifdef KEBABENGINE_EXPORTS
#define KEBAB_API __declspec(dllexport)
#else
#define KEBAB_API __declspec(dllimport)
#endif

// Force the Nvidia GPU to draw Kebab.
// Place before main() function
#define FORCE_NV_OPTIMUS extern "C" { __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }

#include "Types.h"
#include "StringHash.h"