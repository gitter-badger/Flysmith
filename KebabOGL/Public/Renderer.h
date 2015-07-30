#pragma once
#include "PublicDef.h"


namespace cuc
{
	class KEBABOGL_API Renderer
	{
	public:
		Renderer(const HWND windowHandle, const U32 windowWidth, const U32 windowHeight);
		~Renderer();
		void Render();

	private:
		struct Impl;
		Impl* m_pImpl;
	};
}