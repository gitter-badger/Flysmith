#include "PCH.h"
#include "Resource.h"
using namespace cuc;


Resource::Resource()
	: m_pResource(nullptr)
{
}

Resource::~Resource()
{
	if (m_pResource)
	{
		m_pResource->Release();
	}
}

void Resource::CreateCommited(ID3D12Device* pDevice, const ResourceConfig& resConfig, U8** dataPtr, void* pData, size_t dataSize)
{
	assert(pDevice != nullptr);
	assert(m_pResource == nullptr);

	D3D12_HEAP_PROPERTIES heapProperties;
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	HRESULT hr = pDevice->CreateCommittedResource(&heapProperties,
												  D3D12_HEAP_FLAG_NONE,
												  &resConfig.Get(),
												  D3D12_RESOURCE_STATE_GENERIC_READ,
												  nullptr,
												  IID_PPV_ARGS(&m_pResource));
	assert(SUCCEEDED(hr));

	U8* tempDataPtr = nullptr;
	m_pResource->Map(0, nullptr, reinterpret_cast<void**>(&tempDataPtr));
	memcpy(tempDataPtr, pData, dataSize);

	if (dataPtr)
	{
		*dataPtr = tempDataPtr;
	}
	else
	{
		m_pResource->Unmap(0, nullptr);
	}
}

void Resource::Reset()
{
	if (m_pResource)
	{
		m_pResource->Release();
		m_pResource = nullptr;
	}
}

D3D12_GPU_VIRTUAL_ADDRESS Resource::GetGPUVirtualAddress()
{
	assert(m_pResource != nullptr);
	return m_pResource->GetGPUVirtualAddress();
}

ID3D12Resource* Resource::Get()
{
	assert(m_pResource != nullptr);
	return m_pResource;
}