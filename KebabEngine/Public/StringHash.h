#pragma once
#include "PublicDef.h"


// djb2
constexpr U32 Hash(const char* str)
{
	#pragma warning(disable:4307)
	return *str ?
		static_cast<U32>(*str) + 33 * Hash(str + 1) :
		5381;
}

constexpr U32 operator "" _HASH(const char* str, size_t)
{
	return Hash(str);
}