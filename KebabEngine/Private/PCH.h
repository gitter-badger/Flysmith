#pragma once


#define SAFE_RELEASE(x) if(x) \
						{ \
							(x)->Release(); \
							(x) = nullptr; \
						}

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <memory>
#include <unordered_map>
#include <cstdio>
#include <thread>
#include <cassert>
#include <vector>
#include <string>
#include <map>

#include "Types.h"
