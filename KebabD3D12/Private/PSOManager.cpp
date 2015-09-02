#include "PCH.h"
#include "PSOManager.h"


bool PSOManager::s_bInstantiated = false;

PSOManager::PSOManager(Device* pDevice, ResourceCache* pResourceCache, RootSignature* pRootSignature)
	: m_pDevice(pDevice)
	, m_pResCache(pResourceCache)
	, m_pRootSignature(pRootSignature)
	, m_numPSOs(0)
{
	assert(!s_bInstantiated);
	s_bInstantiated = true;
}

PSOManager::~PSOManager()
{
	s_bInstantiated = false;
}

const PipelineStateObject& PSOManager::GetPSOById(U32 id) const
{
	return m_PSOs[id];
}

U32 PSOManager::GetPSOForConfiguration(ResourceHandle vertShaderHandle, ResourceHandle pixelShaderHandle, FillMode fillMode, CullMode cullMode)
{
	auto vertShader = m_pResCache->GetShader(vertShaderHandle),
		 pixelShader = m_pResCache->GetShader(pixelShaderHandle);	

	for (U32 idx = 0; idx < m_numPSOs; ++idx)
	{
		auto& pso = m_PSOs[idx];

		if (pso.GetVertexShader() == vertShader->pShaderBytecode && 
			pso.GetPixelShader() == pixelShader->pShaderBytecode && 
			pso.GetFillMode() == fillMode && 
			pso.GetCullMode() == cullMode)
		{
			return idx;
		}
	}

	PipelineStateObject pso;

	// TODO: Make the Vertex class handle its own layout 
	D3D12_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	RasterizerStateConfig rastState(fillMode, cullMode);
	ShaderPipeline shaderPipeline(vertShader, pixelShader);
	pso.Init(m_pDevice->Get(), layout, 2, shaderPipeline, m_pRootSignature->Get(), &rastState);

	m_PSOs[m_numPSOs++] = pso;
	return m_numPSOs - 1;
}