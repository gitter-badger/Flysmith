#include "PCH.h"
#include "RenderItem.h"
#include "Descriptors\ConstantBufferView.h"
#include "Descriptors\DescriptorHeap.h"


RenderItem::RenderItem(RenderItem&& other)
{
	mesh = other.mesh;
	vertShader = other.vertShader;
	pixelShader = other.pixelShader;
	pWorldMatDataBegin = other.pWorldMatDataBegin;

	other.worldMatConstBuffer.Get()->AddRef();
	worldMatConstBuffer = other.worldMatConstBuffer;
}

RenderItem& RenderItem::operator=(RenderItem&& other)
{
	mesh = other.mesh;
	transform = other.transform;
	vertShader = other.vertShader;
	pixelShader = other.pixelShader;
	pWorldMatDataBegin = other.pWorldMatDataBegin;

	other.worldMatConstBuffer.Get()->AddRef();
	worldMatConstBuffer = other.worldMatConstBuffer;

	return *this;
}

RenderItem::RenderItem()
	: mesh(0)
	, vertShader(0)
	, pixelShader(0)
	, pWorldMatDataBegin(nullptr)
{
}

void RenderItem::Init(ResourceHandle mesh_, ResourceHandle vertShader_, ResourceHandle pixelShader_, ID3D12Device* pDevice, DescriptorHeap* pDescHeap)
{
	mesh = mesh_;
	vertShader = vertShader_;
	pixelShader = pixelShader_;

	// TODO: Consider caching
	ResourceConfig descCB(ResourceType::BUFFER, sizeof(XMFLOAT4X4));
	worldMatConstBuffer.CreateCommited(pDevice, descCB, &pWorldMatDataBegin);

	ConstantBufferView cbView(pDevice, worldMatConstBuffer.GetGPUVirtualAddress(), sizeof(XMFLOAT4X4), pDescHeap->GetCPUHandle(0));
}

void RenderItem::UpdateTransform(const Transform& newTransform)
{
	transform = newTransform;

	XMFLOAT4X4 worldMatTransposed;
	XMStoreFloat4x4(&worldMatTransposed, XMMatrixTranspose(transform.GetMatrixXM()));
	memcpy(&pWorldMatDataBegin, &worldMatTransposed, sizeof(XMFLOAT4X4));
}