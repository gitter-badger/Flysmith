#pragma once
#include "CommandList.h"


struct ID3D12Device;
struct ID3D12CommandAllocator;

class CommandAllocator
{
public:
	CommandAllocator();
	~CommandAllocator();
	void Init(ID3D12Device*, CommandListType = DIRECT);
	ID3D12CommandAllocator* Get();

	void Reset();

private:
	ID3D12CommandAllocator* m_pCommandAllocator;
};