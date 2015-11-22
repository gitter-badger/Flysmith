#pragma once


#ifdef KEBABGL_EXPORTS
#define KEBABGL_API __declspec(dllexport)
#else
#define KEBABGL_API __declspec(dllimport)
#endif
