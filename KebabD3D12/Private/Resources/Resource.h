#pragma once
#include "PCH.h"
#include "ResourceConfig.h"


class Resource
{
public:
	Resource();
	~Resource();

	// Creates both a resource and an implicit heap.
	void CreateCommited(ID3D12Device*, const ResourceConfig&, U8** dataPtr = nullptr, void* pData = nullptr, size_t dataSize = 0, D3D12_HEAP_TYPE = D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_CLEAR_VALUE* = nullptr);
	void Reset();

	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
	ID3D12Resource* Get();

private:
	ID3D12Resource* m_pResource;
};