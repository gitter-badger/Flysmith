#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_DX12
#include <d3d12.h>
#include "Application\Window.h"


namespace cuc
{
	struct Viewport : D3D12_VIEWPORT
	{
		Viewport();
		Viewport(const F32 width, const F32 height, 
				 const F32 topLeftX = 0.0f, const F32 topLeftY = 0.0f, 
				 const F32 minDepth = 0.0f, const F32 maxDepth = 1.0f);

		// Sets the top-left corner to (0, 0) and the bottom-right corner to (windowWidth, windowHeight)
		Viewport(const Window*, const F32 minDepth = 0.0f, const F32 maxDepth = 1.0f);
	};
}

#endif