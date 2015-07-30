#pragma once
#include "PCH.h"


namespace cuc
{
	struct ScissorRectangle : RECT
	{
		ScissorRectangle();
		ScissorRectangle(const U32 bottomRightX, const U32 bottomRightY, const U32 topLeftX = 0, const U32 topLeftY = 0);
		void Resize(const U32 bottomRightX, const U32 bottomRightY, const U32 topLeftX = 0, const U32 topLeftY = 0);
	};
}