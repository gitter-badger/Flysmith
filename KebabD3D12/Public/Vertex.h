#pragma once
#include "PublicDef.h"


namespace cuc
{
	using ResourceHandle = U32;

	struct KEBAB12_API Vertex
	{
		F32 position[3];
		F32 color[4];
	};
}