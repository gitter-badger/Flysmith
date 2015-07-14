#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_DX12
#include <d3d12.h>
#include "Application\Window.h"


namespace cuc
{
	class Window;

	struct ScissorRectangle : RECT
	{
		ScissorRectangle();
		ScissorRectangle(const U32 bottomRightX, const U32 bottomRightY, const U32 topLeftX = 0, const U32 topLeftY = 0);
		void Resize(const U32 bottomRightX, const U32 bottomRightY, const U32 topLeftX = 0, const U32 topLeftY = 0);

		// Sets the top-left corner to (0, 0) and the bottom-right corner to (windowWidth, windowHeight)
		ScissorRectangle(const Window*);
		void Resize(const Window*);
	};
}

#endif