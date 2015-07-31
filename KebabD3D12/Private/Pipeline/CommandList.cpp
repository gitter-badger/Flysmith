#include "PCH.h"
#include "CommandList.h"

cuc::CommandList::CommandList()
	: m_pCommandList(nullptr)
{
}

cuc::CommandList::~CommandList()
{
	if (m_pCommandList)
	{
		m_pCommandList->Release();
	}
}

void cuc::CommandList::Init(ID3D12Device* pDevice, ID3D12CommandAllocator* pAllocator, ID3D12PipelineState* pInitialState, 
							CommandListType type, U32 nodeMask)
{
	assert(pDevice != nullptr);
	assert(pAllocator != nullptr);
	assert(m_pCommandList == nullptr);

	HRESULT hr = pDevice->CreateCommandList(nodeMask, static_cast<D3D12_COMMAND_LIST_TYPE>(type), 
											pAllocator, pInitialState, 
											IID_PPV_ARGS(&m_pCommandList));
	assert(SUCCEEDED(hr));
}

ID3D12GraphicsCommandList* cuc::CommandList::Get()
{
	assert(m_pCommandList != nullptr);
	return m_pCommandList;
}

void cuc::CommandList::Close()
{
	HRESULT hr = m_pCommandList->Close();
	assert(SUCCEEDED(hr));
}

void cuc::CommandList::Reset(ID3D12CommandAllocator* pCommandAllocator, ID3D12PipelineState* pInitialState)
{
	assert(pCommandAllocator != nullptr);

	HRESULT hr = m_pCommandList->Reset(pCommandAllocator, pInitialState);
	assert(SUCCEEDED(hr));
}

void cuc::CommandList::SetRootSignature(ID3D12RootSignature* pRootSignature)
{
	assert(pRootSignature != nullptr);
	m_pCommandList->SetGraphicsRootSignature(pRootSignature);
}

void cuc::CommandList::SetViewports(D3D12_VIEWPORT* pViewports, U32 numViewports)
{
	m_pCommandList->RSSetViewports(numViewports, pViewports);
}

void cuc::CommandList::SetScissorRects(D3D12_RECT* pRects, U32 numRects)
{
	m_pCommandList->RSSetScissorRects(numRects, pRects);
}

void cuc::CommandList::SetPrimitiveTopology(PrimitiveTopology topology)
{
	m_pCommandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(topology));
}

void cuc::CommandList::SetVertexBuffers(D3D12_VERTEX_BUFFER_VIEW* pViews, U32 numViews, U32 startSlot)
{
	m_pCommandList->IASetVertexBuffers(startSlot, numViews, pViews);
}

void cuc::CommandList::SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW* pView)
{
	m_pCommandList->IASetIndexBuffer(pView);
}

void cuc::CommandList::SetPrimitive(PrimitiveTopology topology, D3D12_VERTEX_BUFFER_VIEW* pVertBufView, D3D12_INDEX_BUFFER_VIEW* pIndBufView)
{
	SetPrimitiveTopology(topology);
	SetVertexBuffers(pVertBufView);
	SetIndexBuffer(pIndBufView);
}

void cuc::CommandList::SetRoot32BitConstants(U32 rootParamIndex, U32 numValues, void* pData, U32 offset)
{
	m_pCommandList->SetGraphicsRoot32BitConstants(rootParamIndex, numValues, pData, offset);
}

void cuc::CommandList::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, F32* cleaColor, D3D12_RECT* pRects, U32 numRects)
{
	m_pCommandList->ClearRenderTargetView(rtvHandle, cleaColor, numRects, pRects);
}

void cuc::CommandList::SetRenderTargets(U32 numRenderTargetDescriptors, D3D12_CPU_DESCRIPTOR_HANDLE* pRTVDescriptors, BOOL bSingleHandleToDescriptorRange, D3D12_CPU_DESCRIPTOR_HANDLE * pDepthStencilDescriptor)
{
	m_pCommandList->OMSetRenderTargets(numRenderTargetDescriptors, pRTVDescriptors, bSingleHandleToDescriptorRange, pDepthStencilDescriptor);
}

void cuc::CommandList::DrawIndexedInstanced(U32 numPerInstance, U32 numInstances, U32 startIndexLoc, I32 baseVertLoc, U32 startInstanceLoc)
{
	m_pCommandList->DrawIndexedInstanced(numPerInstance, numInstances, startIndexLoc, baseVertLoc, startInstanceLoc);
}

void cuc::CommandList::DrawIndexed(U32 numIndices, U32 startIndexLoc, I32 baseVertLoc)
{
	DrawIndexedInstanced(numIndices, 1, startIndexLoc, baseVertLoc);
}

void cuc::CommandList::Draw(U32 numVertices, U32 startVertexLoc)
{
	m_pCommandList->DrawInstanced(numVertices, 1, startVertexLoc, 0);
}

void cuc::CommandList::SetResourceBarriers(D3D12_RESOURCE_BARRIER* pBarriers, U32 numBarriers)
{
	m_pCommandList->ResourceBarrier(numBarriers, pBarriers);
}
