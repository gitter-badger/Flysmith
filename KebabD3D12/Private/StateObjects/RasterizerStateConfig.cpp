#include "PCH.h"
#include "RasterizerStateConfig.h"


RasterizerStateConfig::RasterizerStateConfig(FillMode fillMode, CullMode cullMode)
{
	m_desc.FillMode = static_cast<D3D12_FILL_MODE>(fillMode);
	m_desc.CullMode = static_cast<D3D12_CULL_MODE>(cullMode);
	m_desc.FrontCounterClockwise = FALSE;
	m_desc.DepthBias = 0;
	m_desc.SlopeScaledDepthBias = 0.0f;
	m_desc.DepthBiasClamp = 0.0f;
	m_desc.DepthClipEnable = FALSE;
	m_desc.MultisampleEnable = FALSE;
	m_desc.AntialiasedLineEnable = FALSE;
	m_desc.ForcedSampleCount = 0;
	m_desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}

const D3D12_RASTERIZER_DESC& RasterizerStateConfig::Get() const
{
	return m_desc;
}
