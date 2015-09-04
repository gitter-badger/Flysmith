#include "PCH.h"
#include "DepthStencilView.h"


DepthStencilView::DepthStencilView(DXGI_FORMAT format, D3D12_DSV_DIMENSION viewDimension, D3D12_DSV_FLAGS flags)
{
	ZeroMemory(this, sizeof(*this));
	Init(format, viewDimension, flags);
}

void DepthStencilView::Init(DXGI_FORMAT format, D3D12_DSV_DIMENSION viewDimension, D3D12_DSV_FLAGS flags)
{
	Format = format;
	ViewDimension = viewDimension;
	Flags = flags;

	if (viewDimension == D3D12_DSV_DIMENSION_TEXTURE2D)
	{
		Texture2D.MipSlice = 0;
	}
}