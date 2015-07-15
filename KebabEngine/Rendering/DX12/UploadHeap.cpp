#include "UploadHeap.h"
#ifdef RENDERER_DX12
#include <cassert>
using namespace cuc;
using namespace Microsoft::WRL;


const std::map<UploadHeap::Type, U64> UploadHeap::s_heapAlignment = {
	{ Type::BUFFERS_AND_SINGLE_SAMPLE_TEX, 1024 * 64 },          // 64 KiB alignment requirement for buffers and single-sample textures
	{ Type::MULTI_SAMPLE_TEX,              1024 * 1024 * 4 }     // 4  Mib alignment requirement for multi-sample textures
};

UploadHeap::UploadHeap()
	: m_pHeap(nullptr)
	, m_bInitialized(false)
	, m_currentOffset(0)
	, m_size(0)
	, m_pDevice(nullptr)
{
}

UploadHeap::~UploadHeap()
{
	if (m_bInitialized)
	{
		m_pHeap->Release();
		m_pDevice->Release();
	}
}

void UploadHeap::Init(ID3D12Device* pDevice, const U64 size, const Type type)
{
	assert(!m_bInitialized);
	assert(pDevice != nullptr);
	assert(size > 0);

	pDevice->AddRef();
	m_pDevice = pDevice;

	m_size = size * s_heapAlignment.at(type);
	m_type = type;

	D3D12_HEAP_PROPERTIES properties;
	properties.Type = D3D12_HEAP_TYPE_UPLOAD;
	properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	properties.CreationNodeMask = 0;
	properties.VisibleNodeMask = 0;

	D3D12_HEAP_DESC description;
	description.Properties = properties;
	description.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
	description.Alignment = s_heapAlignment.at(type); 
	description.SizeInBytes = m_size;

	HRESULT hr = m_pDevice->CreateHeap(&description, __uuidof(ID3D12Heap), reinterpret_cast<void**>(&m_pHeap));
	assert(SUCCEEDED(hr));
	
	m_bInitialized = true;
}

void UploadHeap::Alloc(ID3D12Resource** ppResource, const D3D12_RESOURCE_DESC& desc, void* data, const size_t dataSize, 
					   const D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue)
{
	assert(m_currentOffset < m_size);
	HRESULT hr = m_pDevice->CreatePlacedResource(m_pHeap, m_currentOffset, &desc, initialState, clearValue, __uuidof(ID3D12Resource), reinterpret_cast<void**>(ppResource));
	assert(SUCCEEDED(hr));
	m_currentOffset += s_heapAlignment.at(m_type);

	UINT8* dataBegin = nullptr;
	(*ppResource)->Map(0, nullptr, reinterpret_cast<void**>(&dataBegin));
	memcpy(dataBegin, data, dataSize);
	(*ppResource)->Unmap(0, nullptr);
}

ID3D12Heap* UploadHeap::Get()
{
	return m_pHeap;
}

#endif