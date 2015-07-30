#pragma once
#include "PublicDef.h"
#include "Mesh.h"


namespace cuc
{
	class KEBAB12_API Renderer
	{
	public:
		Renderer(const HWND, const U32 windowWidth, const U32 windowHeight);
		~Renderer();
		void Render();

		void SubmitMesh(Mesh);

	private:
		struct Impl;
		Impl* m_pImpl;
	};
}