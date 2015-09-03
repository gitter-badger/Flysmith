#pragma once
#include "Component.h"
#include "../../KebabD3D12/Public/Transform.h"


class Renderer;

class KEBAB_API RenderComponent : public Component
{
public:
	RenderComponent(Renderer*, U32 mesh, U32 vertShader, U32 pixelShader);
	U32 GetRenderItem() const;

	Transform m_transform;

private:
	U32 m_renderItem;
};