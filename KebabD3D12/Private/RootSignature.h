#pragma once
#include "PCH.h"


// TODO: May remove the outside factory thing altogether.
class RootSignature
{
public:
	RootSignature() : m_pRootSignature(nullptr)
	{
	}

	RootSignature(const RootSignature&) = delete;
	RootSignature& operator=(const RootSignature&) = delete;

	~RootSignature()
	{
		if (m_pRootSignature)
		{
			m_pRootSignature->Release();
			m_pRootSignature = nullptr;
		}
	}

	void Init(ID3D12RootSignature* pRootSignature)
	{
		m_pRootSignature = pRootSignature;
	}

	ID3D12RootSignature* Get() const
	{
		assert(m_pRootSignature);
		return m_pRootSignature;
	}

private:
	ID3D12RootSignature* m_pRootSignature;
};