#pragma once
#include "PCH.h"
#include "GLContext.h"
#include <memory>


namespace cuc
{
	class KEBABOGL_API GLRenderer
	{
	public:
		GLRenderer(HWND windowHandle, const U32 windowWidth, const U32 windowHeight);
		~GLRenderer();
		void Render();

	private:
		GLContext m_context;
	};
}