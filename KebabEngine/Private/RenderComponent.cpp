#include "PCH.h"
#include "RenderComponent.h"
#include "../../KebabD3D12/Public/Renderer.h"


RenderComponent::RenderComponent(Renderer* pRenderer, U32 mesh, U32 vertShader, U32 pixelShader)
{
	m_renderItem = pRenderer->AddRenderItem(mesh, vertShader, pixelShader);
}

U32 RenderComponent::GetRenderItem() const
{
	return m_renderItem;
}