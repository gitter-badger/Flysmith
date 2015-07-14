#include "Viewport.h"
#ifdef RENDERER_DX12
#include <cassert>
using namespace cuc;


Viewport::Viewport()
{
	Width = 1.0f;
	Height = 1.0f;
	TopLeftX = 0.0f;
	TopLeftY = 0.0f;
	MinDepth = 0.0f;
	MaxDepth = 1.0f;
}

Viewport::Viewport(const F32 width, const F32 height, const F32 topLeftX, const F32 topLeftY, const F32 minDepth, const F32 maxDepth)
{
	Resize(width, height, topLeftX, topLeftY, minDepth, maxDepth);
}

Viewport::Viewport(const Window* pWindow, const F32 minDepth, const F32 maxDepth)
{
	Resize(pWindow, minDepth, maxDepth);
}

void Viewport::Resize(const F32 width, const F32 height, const F32 topLeftX, const F32 topLeftY, const F32 minDepth, const F32 maxDepth)
{
	Width = width;
	Height = height;
	TopLeftX = topLeftX;
	TopLeftY = topLeftY;
	MinDepth = minDepth;
	MaxDepth = maxDepth;
}

void Viewport::Resize(const Window* pWindow, const F32 minDepth, const F32 maxDepth)
{
	assert(pWindow != nullptr);

	Width = static_cast<F32>(pWindow->GetWidth());
	Height = static_cast<F32>(pWindow->GetHeight());
	TopLeftX = 0.0f;
	TopLeftY = 0.0f;
	MinDepth = minDepth;
	MaxDepth = maxDepth;
}

#endif