#pragma once
#include "PCH.h"


namespace cuc
{
	class KEBAB12_API Renderer
	{
	public:
		Renderer(const HWND, const U32 windowWidth, const U32 windowHeight);
		~Renderer();
		void Render();

	private:
		struct Impl;
		Impl* m_pImpl;
	};
}