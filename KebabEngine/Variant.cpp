#include "Variant.h"
using namespace cuc;


inline Variant32::Variant32()
	: type(Type::INVALID)
	, asInteger(0)
	, asFloat(0.0f)
{
}

inline Variant32::Variant32(F32 value)
	: type(Type::FLOAT)
	, asFloat(value)
{
}

inline Variant32::Variant32(I32 value)
	: type(Type::INTEGER)
	, asInteger(value)
{
}

inline void Variant32::SetValue(F32 value)
{
	type = Type::FLOAT;
	asFloat = value;
}

inline void Variant32::SetValue(I32 value)
{
	type = Type::INTEGER;
	asInteger = value;
}