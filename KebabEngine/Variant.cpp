#include "Variant.h"
using namespace cuc;


inline Variant32::Variant32()
	: type(Type::INVALID)
{
}

inline Variant32::Variant32(F32 value)
	: type(Type::FLOAT)
	, asFloat(value)
{
}

inline Variant32::Variant32(I32 value)
	: type(Type::SIGNED_INT)
	, asSignedInt(value)
{
}

inline Variant32::Variant32(U32 value)
	: type(Type::UNSIGNED_INT)
	, asUnsignedInt(value)
{
}

inline void Variant32::SetValue(F32 value)
{
	type = Type::FLOAT;
	asFloat = value;
}

inline void Variant32::SetValue(I32 value)
{
	type = Type::SIGNED_INT;
	asSignedInt = value;
}

inline void Variant32::SetValue(U32 value)
{
	type = Type::UNSIGNED_INT;
	asUnsignedInt = value;
}