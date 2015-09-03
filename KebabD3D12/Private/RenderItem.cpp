#include "PCH.h"
#include "RenderItem.h"
#include "Descriptors\ConstantBufferView.h"
#include "Descriptors\DescriptorHeap.h"
#include "Resources\ResourceCache.h"
using namespace DirectX;


RenderItem::RenderItem(RenderItem&& other)
{
	transform = other.transform;
	mesh = other.mesh;
	pWorldMatDataBegin = other.pWorldMatDataBegin;

	other.worldMatConstBuffer.Get()->AddRef();
	worldMatConstBuffer = other.worldMatConstBuffer;
}

RenderItem& RenderItem::operator=(RenderItem&& other)
{
	transform = other.transform;
	mesh = other.mesh;
	pWorldMatDataBegin = other.pWorldMatDataBegin;

	other.worldMatConstBuffer.Get()->AddRef();
	worldMatConstBuffer = other.worldMatConstBuffer;

	return *this;
}

RenderItem::RenderItem()
	: mesh(0)
	, pWorldMatDataBegin(nullptr)
{
}

void RenderItem::Init(ResourceHandle mesh_, ResourceHandle vertShader, ResourceHandle pixelShader, ID3D12Device* pDevice, D3D12_CPU_DESCRIPTOR_HANDLE descHeapSlot, PSOManager* psoManager, FillMode fillMode, CullMode cullMode)
{
	mesh = mesh_;

	// TODO: Consider caching
	ResourceConfig descCB(ResourceType::BUFFER, sizeof(XMFLOAT4X4));
	worldMatConstBuffer.CreateCommited(pDevice, descCB, &pWorldMatDataBegin);
	ConstantBufferView cbView(pDevice, worldMatConstBuffer.GetGPUVirtualAddress(), sizeof(XMFLOAT4X4), descHeapSlot);

	psoId = psoManager->GetPSOForConfiguration(vertShader, pixelShader, fillMode, cullMode);
}

void RenderItem::UpdateTransform(const XMFLOAT4X4& newTransform)
{
	transform = newTransform;

	XMFLOAT4X4 worldMatTransposed;
	XMStoreFloat4x4(&worldMatTransposed, XMMatrixTranspose(XMLoadFloat4x4(&transform)));
	memcpy(pWorldMatDataBegin, &worldMatTransposed, sizeof(XMFLOAT4X4));
}