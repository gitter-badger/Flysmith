#pragma once

#include "Types.h"

#ifdef KEBABD3D12_EXPORTS
#define KEBAB12_API __declspec(dllexport)
#else
#define KEBAB12_API __declspec(dllimport)
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d12.h>
#include <wrl\client.h>

#include <map>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_set>