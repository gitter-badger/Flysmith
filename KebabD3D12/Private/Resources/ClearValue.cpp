#include "PCH.h"
#include "ClearValue.h"


ClearValue::ClearValue(DXGI_FORMAT format, F32 color[4])
{
	Format = format;
	memcpy(&Color, color, 4 * sizeof(F32));
}

ClearValue::ClearValue(DXGI_FORMAT format, F32 depth, U8 stencil)
{
	Format = format;
	DepthStencil.Depth = depth;
	DepthStencil.Stencil = stencil;
}