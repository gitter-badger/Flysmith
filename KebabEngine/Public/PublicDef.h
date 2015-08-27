#pragma once

// Happens with STL objects 
#pragma warning(disable:4251)

#ifdef KEBABENGINE_EXPORTS
#define KEBAB_API __declspec(dllexport)
#else
#define KEBAB_API __declspec(dllimport)
#endif

#include "Types.h"
#include "StringHash.h"