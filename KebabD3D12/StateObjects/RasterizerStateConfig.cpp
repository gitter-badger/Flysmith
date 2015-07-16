#include "PCH.h"
#include "RasterizerStateConfig.h"
using namespace cuc;


RasterizerStateConfig::RasterizerStateConfig(const D3D12_FILL_MODE fillMode, const D3D12_CULL_MODE cullMode)
{
	FillMode = fillMode;
	CullMode = cullMode;
	FrontCounterClockwise = FALSE;
	DepthBias = 0;
	SlopeScaledDepthBias = 0.0f;
	DepthBiasClamp = 0.0f;
	DepthClipEnable = TRUE;
	MultisampleEnable = FALSE;
	AntialiasedLineEnable = FALSE;
	ForcedSampleCount = 0;
	ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}