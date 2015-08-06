#pragma once
#include "PCH.h"
#include "ResourceConfig.h"


namespace cuc
{
	class Resource
	{
	public:
		Resource();
		~Resource();

		// Creates both a resource and an implicit heap.
		void CreateCommited(ID3D12Device*, const ResourceConfig&, U8** dataPtr, void* pData = nullptr, size_t dataSize = 0);
		void Reset();

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
		ID3D12Resource* Get();

	private:
		ID3D12Resource* m_pResource;
	};
}