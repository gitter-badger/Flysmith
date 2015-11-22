#pragma once
#include "PublicDef.h"
#include <DirectXMath.h>


// Used to copy render state from the simulation system to the renderer. 
struct KEBABCOMMON_API RenderItemProxy
{
	RenderItemProxy(const DirectX::XMFLOAT4X4 transform, const U32 renderItemId);
	const DirectX::XMFLOAT4X4 transform;
	const U32 renderItemId;
};