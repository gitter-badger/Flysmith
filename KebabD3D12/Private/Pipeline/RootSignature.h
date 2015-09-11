#pragma once
#include "PCH.h"


// TODO: May remove the outside factory thing altogether.
class RootSignature
{
public:
	RootSignature();
	~RootSignature();
	void Init(ID3D12RootSignature* pRootSignature);

	RootSignature(const RootSignature&) = delete;
	RootSignature& operator=(const RootSignature&) = delete;

	ID3D12RootSignature* Get() const;

private:
	ID3D12RootSignature* m_pRootSignature;
};