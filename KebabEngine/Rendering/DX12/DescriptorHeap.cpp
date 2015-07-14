#include "DescriptorHeap.h"
#ifdef RENDERER_DX12
#include <cassert>
using namespace cuc;


DescriptorHeap::DescriptorHeap()
	: m_pDescHeap(nullptr)
	, m_startHandleCPU{ 0 }
	, m_startHandleGPU{ 0 }
	, m_handleIncrement(0)
	, m_bInitialized(false)	
{
}

DescriptorHeap::~DescriptorHeap()
{
	m_pDescHeap->Release();
}

void DescriptorHeap::Init(ID3D12Device* pDevice, DescHeapType type)
{
	assert(!m_bInitialized);

	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.Type = static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(type);

	if (type == DescHeapType::CB_SR_UA || type == DescHeapType::SAMPLER)
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	else
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	desc.NumDescriptors = 1;
	desc.NodeMask = 0;

	HRESULT hr = pDevice->CreateDescriptorHeap(&desc, __uuidof(ID3D12DescriptorHeap), reinterpret_cast<void**>(&m_pDescHeap));
	assert(SUCCEEDED(hr));

	m_startHandleCPU = m_pDescHeap->GetCPUDescriptorHandleForHeapStart();
	m_startHandleGPU = m_pDescHeap->GetGPUDescriptorHandleForHeapStart();

	m_handleIncrement = pDevice->GetDescriptorHandleIncrementSize(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(type));

	m_bInitialized = true;
}

const bool DescriptorHeap::IsInitialized() const
{
	return m_bInitialized;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCPUHandle(U32 index) const
{
	assert(m_bInitialized);

	D3D12_CPU_DESCRIPTOR_HANDLE handle{ m_startHandleCPU.ptr + (index * m_handleIncrement) };
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUHandle(U32 index) const
{
	assert(m_bInitialized);
	// TODO: check if the heap is visible to shaders.

	D3D12_GPU_DESCRIPTOR_HANDLE handle{ m_startHandleGPU.ptr + (index * m_handleIncrement) };
	return handle;
}

#endif