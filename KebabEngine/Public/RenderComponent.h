#pragma once
#include "Component.h"


class Renderer;

class KEBAB_API RenderComponent : public Component
{
public:
	RenderComponent(Renderer*, U32 mesh, U32 vertShader, U32 pixelShader);
	
	U32 GetRenderItem() const;
	Type GetType() const;

private:
	U32 m_renderItem;
};