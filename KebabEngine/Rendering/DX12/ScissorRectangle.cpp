#include "ScissorRectangle.h"
#ifdef RENDERER_DX12
#include <cassert>
using namespace cuc;


ScissorRectangle::ScissorRectangle()
{
	left = 0;
	top = 0;
	right = 1;
	bottom = 1;
}

ScissorRectangle::ScissorRectangle(const U32 bottomRightX, const U32 bottomRightY, const U32 topLeftX, const U32 topLeftY)
{
	Resize(bottomRightX, bottomRightY, topLeftX, topLeftY);
}

void ScissorRectangle::Resize(const U32 bottomRightX, const U32 bottomRightY, const U32 topLeftX, const U32 topLeftY)
{
	left = topLeftX;
	top = topLeftY;
	right = bottomRightX;
	bottom = bottomRightY;
}

ScissorRectangle::ScissorRectangle(const Window* pWindow)
{
	Resize(pWindow);
}

void ScissorRectangle::Resize(const Window* pWindow)
{
	assert(pWindow != nullptr);

	left = 0;
	top = 0;
	right = pWindow->GetWidth();
	bottom = pWindow->GetHeight();
}

#endif