#pragma once
#include "PublicDef.h"


enum class DataType : U32
{
	UNKNOWN,
	U8,
	U16,
	U32,
	BOOL,
	FLOAT
};

struct KEBAB_API EventData final
{
	EventData() : type(DataType::UNKNOWN) {}
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

// length - 4 bytes
// type   - 4 bytes
// data   - 56 bytes
// Total  - 64 bytes / Event 
struct KEBAB_API Event final
{
	U32       length;
	U32       type;
	EventData data[MAX_EVENT_DATA];

	Event();
};

inline Event::Event() : type("UNKNOWN"_HASH), length(0) { }