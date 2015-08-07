#pragma once
#include "PublicDef.h"
#include "Transform.h"
#include "Vertex.h"


namespace cuc
{
	class KEBAB12_API Renderer
	{
	public:
		Renderer(const HWND, const U32 windowWidth, const U32 windowHeight);
		~Renderer();

		// Copy visible render components
		void UpdateScene(const Transform& tempSingleEntity);

		// Copy camera transform
		void UpdateView(const TransformNoScale& camTransform);
		
		void Render();

		// TODO: Queue resource caching requests for later 
		ResourceHandle CacheMesh(const std::vector<Vertex>& verts, const std::vector<U32>& indices);
		
	private:
		struct Impl;
		Impl* m_pImpl;
	};
}