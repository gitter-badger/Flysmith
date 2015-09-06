#pragma once
#include "PublicDef.h"


struct KEBAB12_API DirectX::XMFLOAT4X4;

// Used to copy render state from the simulation system to the renderer. 
struct KEBAB12_API RenderItemProxy
{
	RenderItemProxy(const DirectX::XMFLOAT4X4 transform, const U32 renderItemId);
	const DirectX::XMFLOAT4X4 transform;
	const U32 renderItemId;
};