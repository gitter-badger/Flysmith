#include "PCH.h"
#include "BlendStateConfig.h"
using namespace cuc;


BlendStateConfig::BlendStateConfig()
{
	AlphaToCoverageEnable = FALSE;
	IndependentBlendEnable = FALSE;
	// Using only one render target.
	RenderTarget[0].BlendEnable = FALSE;
	RenderTarget[0].BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	RenderTarget[0].LogicOpEnable = FALSE;
	RenderTarget[0].LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	RenderTarget[0].SrcBlend = D3D12_BLEND::D3D12_BLEND_ONE;
	RenderTarget[0].DestBlend = D3D12_BLEND::D3D12_BLEND_ZERO;
	RenderTarget[0].SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	RenderTarget[0].DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ZERO;
	RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
}