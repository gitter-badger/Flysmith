#pragma once
#include "PublicDef.h"
#include "../../KebabCommon/Transform.h"
#include "../../KebabCommon/Vertex.h"
#include "ShaderTypes.h"
#include "RenderItemProxy.h"


using RenderItemHandle = U32;
class RenderComponent;

class KEBAB12_API Renderer
{
public:
	Renderer(const HWND, const U32 windowWidth, const U32 windowHeight);
	~Renderer();

	// Populate a list of render item proxies from render component data and update 
	// the render state for the next frame.
	void UpdateScene(std::vector<RenderItemProxy>) const;

	// Copy camera transform
	void UpdateView(const TransformNoScale& camTransform) const;

	void Render() const;

	RenderItemHandle AddRenderItem(ResourceHandle mesh, ResourceHandle vertexShader, ResourceHandle pixelShader) const;

	// TODO: Queue resource caching requests for later 
	ResourceHandle CacheMesh(const VertexArray& verts, const IndexArray& indices) const;
	void UpdateMesh(ResourceHandle, const VertexArray& verts, const IndexArray& indices) const;
	ResourceHandle CacheShader(ShaderType, const std::wstring& fullPath) const;

private:
	struct Impl;
	Impl* m_pImpl;
};