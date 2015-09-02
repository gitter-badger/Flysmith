#pragma once
#include "Transform.h"
#include "Renderer.h"
#include "Component.h"


class KEBAB12_API RenderComponent : public Component
{
public:
	RenderComponent(Renderer*, U32 mesh, U32 vertShader, U32 pixelShader);
	U32 GetRenderItem() const;

	Transform m_transform;

private:
	U32 m_renderItem;
};