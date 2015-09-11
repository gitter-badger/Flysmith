#pragma once
#include "PCH.h"


enum class FenceFlags
{
	NONE = 0,
	SHARED = 0x1,
	SHARED_CROSS_ADAPTER = 0x2
};

class Fence
{
public:
	Fence();
	~Fence();
	void Init(ID3D12Device* pDevice, U64 initialValue, FenceFlags = FenceFlags::NONE);
	ID3D12Fence* Get();

	U64 GetCompletedValue();
	void SetEventOnCompletion(U64 value, HANDLE hEvent);
	void Signal(U64 value);

private:
	ID3D12Fence* m_pFence;
};