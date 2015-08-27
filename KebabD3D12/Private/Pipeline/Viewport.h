#pragma once
#include "PCH.h"


struct Viewport : D3D12_VIEWPORT
{
	Viewport();
	Viewport(const F32 width, const F32 height,
		const F32 topLeftX = 0.0f, const F32 topLeftY = 0.0f,
		const F32 minDepth = 0.0f, const F32 maxDepth = 1.0f);
	void Resize(const F32 width, const F32 height,
		const F32 topLeftX = 0.0f, const F32 topLeftY = 0.0f,
		const F32 minDepth = 0.0f, const F32 maxDepth = 1.0f);
};