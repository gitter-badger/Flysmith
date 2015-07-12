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
// TODO: There seems to be an issue when mixing D3D12 with the nvidia adapter.
#ifndef RENDERER_DX12
#define FORCE_NV_OPTIMUS extern "C" { __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }
#endif

#include "Types.h"
#include "StringHash.h"