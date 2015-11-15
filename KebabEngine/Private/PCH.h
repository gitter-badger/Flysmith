#pragma once


#define SAFE_RELEASE(x) if(x)               \
						{                   \
							(x)->Release(); \
							(x) = nullptr;  \
						}

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include <unordered_map>
#include <cassert>
#include <fstream>
#include <memory>
#include <cstdio>
#include <thread>
#include <vector>
#include <string>
#include <map>

#include "Types.h"
