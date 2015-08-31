#pragma once
#include "Transform.h"
#include "Resources\Resource.h"


class DescriptorHeap;

struct RenderItem
{
	RenderItem();
	RenderItem(RenderItem&&);
	RenderItem& operator=(RenderItem&&);
	RenderItem(const RenderItem&&) = delete;
	RenderItem& operator=(const RenderItem&) = delete;

	void Init(ResourceHandle mesh_, ResourceHandle vertShader_, ResourceHandle pixelShader_, ID3D12Device* pDevice, DescriptorHeap* pDescHeap);
	void UpdateTransform(const Transform&);

	Transform transform;
	ResourceHandle mesh;
	ResourceHandle vertShader;
	ResourceHandle pixelShader;

	Resource worldMatConstBuffer;
	U8*      pWorldMatDataBegin;
};