#pragma once
#include "PublicDef.h"
#include "Transform.h"
#include "Mesh.h"


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

		void SubmitMesh(Mesh);

	private:
		struct Impl;
		Impl* m_pImpl;
	};
}