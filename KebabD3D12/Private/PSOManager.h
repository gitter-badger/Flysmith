#pragma once
#include "PCH.h"
#include "Device.h"
#include "RootSignature.h"
#include "Resources\ResourceCache.h"
#include "Pipeline\PipelineStateObject.h"


class PSOManager
{
public:
	PSOManager(Device*, ResourceCache*, RootSignature*);
	~PSOManager();

	const PipelineStateObject& GetPSOById(U32 id) const;
	U32 GetPSOForConfiguration(ResourceHandle vertShaderHandle, ResourceHandle pixelShaderHandle, FillMode fillMode = FillMode::SOLID, CullMode cullMode = CullMode::BACK);

private:
	Device* m_pDevice;
	ResourceCache* m_pResCache;
	RootSignature* m_pRootSignature;
	static const size_t MAX_PSO_CONFIGS = 10;
	PipelineStateObject m_PSOs[MAX_PSO_CONFIGS];
	size_t m_numPSOs;
	static bool s_bInstantiated;
};