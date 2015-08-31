#include "PCH.h"
#include "RenderItem.h"
#include "Descriptors\ConstantBufferView.h"
#include "Descriptors\DescriptorHeap.h"
#include "Resources\ResourceCache.h"


RenderItem::RenderItem(RenderItem&& other)
{
	transform = other.transform;
	mesh = other.mesh;
	vertShader = other.vertShader;
	pixelShader = other.pixelShader;
	pWorldMatDataBegin = other.pWorldMatDataBegin;

	other.worldMatConstBuffer.Get()->AddRef();
	worldMatConstBuffer = other.worldMatConstBuffer;
}

RenderItem& RenderItem::operator=(RenderItem&& other)
{
	transform = other.transform;
	mesh = other.mesh;
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

void RenderItem::Init(ResourceHandle mesh_, ResourceHandle vertShader_, ResourceHandle pixelShader_, ID3D12Device* pDevice, DescriptorHeap* pDescHeap, ID3D12RootSignature* pRootSignature, ResourceCache* pResCache)
{
	mesh = mesh_;
	vertShader = vertShader_;
	pixelShader = pixelShader_;

	// TODO: Consider caching
	ResourceConfig descCB(ResourceType::BUFFER, sizeof(XMFLOAT4X4));
	worldMatConstBuffer.CreateCommited(pDevice, descCB, &pWorldMatDataBegin);
	ConstantBufferView cbView(pDevice, worldMatConstBuffer.GetGPUVirtualAddress(), sizeof(XMFLOAT4X4), pDescHeap->GetCPUHandle(0));

	D3D12_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	RasterizerStateConfig rastState(D3D12_FILL_MODE_WIREFRAME, D3D12_CULL_MODE_NONE);
	pso.Init(pDevice, layout, 2, pRootSignature, nullptr, &rastState, &pResCache->GetShader(0), &pResCache->GetShader(1));
}

void RenderItem::UpdateTransform(const Transform& newTransform)
{
	transform = newTransform;

	XMFLOAT4X4 worldMatTransposed;
	XMStoreFloat4x4(&worldMatTransposed, XMMatrixTranspose(transform.GetMatrixXM()));
	memcpy(pWorldMatDataBegin, &worldMatTransposed, sizeof(XMFLOAT4X4));
}