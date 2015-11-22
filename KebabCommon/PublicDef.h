#pragma once
#ifdef KEBABCOMMON_EXPORTS
#define KEBABCOMMON_API __declspec(dllexport)
#else
#define KEBABCOMMON_API __declspec(dllimport)
#endif

#include "Types.h"

// TEMP
// Will reinvent the wheel at some point :^)
#include <DirectXMath.h>