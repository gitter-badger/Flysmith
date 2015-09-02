#pragma once
#include "Transform.h"
#include "Resources\Resource.h"
#include "StateObjects\RasterizerStateConfig.h"
#include "PSOManager.h"


class DescriptorHeap;
class ResourceCache;

// TODO: Handle state changes to render items outside -> make it a POD 
struct RenderItem
{
	RenderItem();
	RenderItem(RenderItem&&);
	RenderItem& operator=(RenderItem&&);
	RenderItem(const RenderItem&&) = delete;
	RenderItem& operator=(const RenderItem&) = delete;

	void Init(ResourceHandle mesh_, ResourceHandle vertShader, ResourceHandle pixelShader, ID3D12Device* pDevice, D3D12_CPU_DESCRIPTOR_HANDLE descHeapSlot, PSOManager*, FillMode = FillMode::SOLID, CullMode = CullMode::BACK);
	void UpdateTransform(const Transform&);

	Transform transform;
	ResourceHandle mesh;

	Resource worldMatConstBuffer;
	U8*      pWorldMatDataBegin;

	U32 psoId;
};