#include "PCH.h"
#include "RootSignature.h"


RootSignature::RootSignature() 
	: m_pRootSignature(nullptr)
{
}

RootSignature::~RootSignature()
{
	if (m_pRootSignature)
	{
		m_pRootSignature->Release();
		m_pRootSignature = nullptr;
	}
}

void RootSignature::Init(ID3D12RootSignature* pRootSignature)
{
	m_pRootSignature = pRootSignature;
}

ID3D12RootSignature* RootSignature::Get() const
{
	assert(m_pRootSignature);
	return m_pRootSignature;
}