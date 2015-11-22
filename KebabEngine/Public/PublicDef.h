#pragma once

// Happens with STL objects 
#pragma warning(disable:4251)

#include "RendererSwitch.h"

#ifdef KEBABENGINE_EXPORTS
#define KEBAB_API __declspec(dllexport)
#else
#define KEBAB_API __declspec(dllimport)
#endif

// Force the Nvidia GPU to draw Kebab(valid only when using OpenGL)
// Place before main() function.
#ifdef RENDERER_GL
#define FORCE_NV_OPTIMUS extern "C" { __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }
#else
#define FORCE_NV_OPTIMUS
#endif


#include "../../KebabCommon/Types.h"
#include "StringHash.h"