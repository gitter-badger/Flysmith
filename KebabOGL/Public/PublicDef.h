#pragma once


#include "Types.h"

#ifdef KEBABOGL_EXPORTS
#define KEBABOGL_API __declspec(dllexport)
#else
#define KEBABOGL_API __declspec(dllimport)
#endif