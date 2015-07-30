#pragma once
#include "PublicDef.h"


namespace cuc
{
	// Setting values like this may end up in mismatching types and values:
	//		variant.type = FLOAT;
	//		variant.asInteger = 12;
	//		if(variant.type == FLOAT) 
	//			variant.asFloat++; // Gets evaluated. Last value written to was asInteger, resulting in undefined behavior here.
	//
	// Use the the constructors and setter methods instead:
	//		Variant32 variant(12.0f);
	//		variant.SetValue(15);
	struct KEBAB_API Variant32
	{
		enum class Type
		{
			INVALID,
			SIGNED_INT,
			UNSIGNED_INT,
			FLOAT
		} type;

		Variant32();
		Variant32(F32);
		Variant32(I32);
		Variant32(U32);
		
		void SetValue(F32);
		void SetValue(I32);
		void SetValue(U32);

		F32 GetFloat() const;
		I32 GetSignedInt() const;
		U32 GetUnsignedInt() const;

		bool operator ==(const Variant32&);

	private:
		union
		{
			I32 asSignedInt;
			U32 asUnsignedInt;
			F32 asFloat;
		};
	};
}