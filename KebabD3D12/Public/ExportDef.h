#pragma once
#ifdef KEBABD3D12_EXPORTS
#define KEBAB12_API __declspec(dllexport)
#else
#define KEBAB12_API __declspec(dllimport)
#endif