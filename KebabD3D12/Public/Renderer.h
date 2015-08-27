#pragma once
#include "PublicDef.h"
#include "Transform.h"
#include "Vertex.h"
#include "ShaderTypes.h"


using RenderItemHandle = U32;
class RenderComponent;

class KEBAB12_API Renderer
{
public:
	Renderer(const HWND, const U32 windowWidth, const U32 windowHeight);
	~Renderer();

	// Copy visible render components
	void UpdateScene(const std::vector<RenderComponent>&);

	// Copy camera transform
	void UpdateView(const TransformNoScale& camTransform);

	void Render();

	RenderItemHandle AddRenderItem(ResourceHandle mesh, ResourceHandle vertexShader, ResourceHandle pixelShader);

	// TODO: Queue resource caching requests for later 
	ResourceHandle CacheMesh(const std::vector<Vertex>& verts, const std::vector<U32>& indices);
	ResourceHandle CacheShader(ShaderType, const std::wstring& fullPath);

private:
	struct Impl;
	Impl* m_pImpl;
};