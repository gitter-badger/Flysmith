#pragma once
#include "PublicDef.h"
#include "Variant.h"
#include <vector>


namespace cuc
{
	enum class DataType : U32
	{
		U8,
		U16,
		U32,
		BOOL,
		FLOAT
	};

	struct EventData
	{
		EventData() {}
		explicit EventData(U8 data) : type(DataType::U8) { asU8 = data; }
		explicit EventData(U16 data) : type(DataType::U16) { asU16 = data; }
		explicit EventData(U32 data) : type(DataType::U32) { asU32 = data; }
		explicit EventData(bool data) : type(DataType::BOOL) { asBool = data; }
		explicit EventData(float data) : type(DataType::FLOAT) { asFloat = data; }

		DataType type; // 4 bytes
		union
		{
			U8    asU8;
			U16   asU16;
			U32   asU32;
			bool  asBool;
			float asFloat;
		}; // 4 bytes
	};

	// Maximum amount of 'data', or arguments, an event may have.
	const U32 MAX_EVENT_DATA = 7;

	// Hashed human-readable event name
	using EventType = U32;

	// length - 4 bytes
	// type   - 4 bytes
	// data   - 56 bytes
	// Total  - 64 bytes / Event 
	struct Event final
	{
		U32       length;
		EventType type;
		EventData data[MAX_EVENT_DATA];

		Event();
	};

	inline Event::Event() : type("UNKNOWN"_HASH), length(0) { }
};
