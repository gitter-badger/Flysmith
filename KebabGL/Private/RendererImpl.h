#pragma once
#include "Renderer.h"
#include "GLContext.h"

#include "../../KebabCommon/Camera.h"
#include "../../KebabCommon/PerspectiveMatrix.h"


struct Renderer::Impl
{
	Impl(HWND windowHandle, const U32 windowWidth, const U32 windowHeight);

	GLContext m_context;

	static const size_t MAX_RENDER_QUEUE_ITEMS = 100;

	// Holds ids of render items that are going to be rendered this frame.
	U32 m_renderQueue[MAX_RENDER_QUEUE_ITEMS];
	U32 m_renderQueueEnd;

	// Holds all render items, whether they are to be rendered or not.
	/*std::vector<RenderItem> m_renderItems;*/
	
	Camera m_camera;
	PerspectiveMatrix m_projPerspective;

	struct RenderItemCacheRequest
	{
		ResourceHandle shader;
		ResourceHandle mesh;
	};
	std::vector<RenderItemCacheRequest> m_renderItemCacheRequets;

	struct UIRenderItemCacheRequest
	{
		ResourceHandle texture;
	};
	std::vector<UIRenderItemCacheRequest> m_uiRenderItemCacheRequets;
};