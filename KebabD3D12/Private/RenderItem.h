#pragma once
#include "Transform.h"


struct RenderItem
{
	RenderItem(ResourceHandle mesh_, ResourceHandle vertShader_, ResourceHandle pixelShader_)
		: mesh(mesh_), vertShader(vertShader_), pixelShader(pixelShader_) {}

	Transform transform;
	ResourceHandle mesh;
	ResourceHandle vertShader;
	ResourceHandle pixelShader;
};