#include "PCH.h"
#include "Viewport.h"
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

void Viewport::Resize(const F32 width, const F32 height, const F32 topLeftX, const F32 topLeftY, const F32 minDepth, const F32 maxDepth)
{
	Width = width;
	Height = height;
	TopLeftX = topLeftX;
	TopLeftY = topLeftY;
	MinDepth = minDepth;
	MaxDepth = maxDepth;
}