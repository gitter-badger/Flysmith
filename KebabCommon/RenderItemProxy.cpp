#include "RenderItemProxy.h"


RenderItemProxy::RenderItemProxy(const DirectX::XMFLOAT4X4 transform_, const U32 renderItemId_)
	: transform(transform_), renderItemId(renderItemId_)
{
}