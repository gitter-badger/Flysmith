#include "PCH.h"
#include "Variant.h"


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

bool Variant32::operator==(const Variant32& rhs)
{
	if (type != rhs.type)
		return false;
	
	switch (type)
	{
	case Type::FLOAT:
		if (asFloat != rhs.asFloat)
			return false;
		break;
	case Type::SIGNED_INT:
		if (asSignedInt != rhs.asSignedInt)
			return false;
		break;
	case Type::UNSIGNED_INT:
		if (asUnsignedInt != rhs.asUnsignedInt)
			return false;
		break;
	}

	return true;
}

inline F32 Variant32::GetFloat() const
{
	assert(type == Type::FLOAT);
	return asFloat;
}

inline I32 Variant32::GetSignedInt() const
{
	assert(type == Type::SIGNED_INT);
	return asSignedInt;
}

inline U32 Variant32::GetUnsignedInt() const
{
	assert(type == Type::UNSIGNED_INT);
	return asUnsignedInt;
}
