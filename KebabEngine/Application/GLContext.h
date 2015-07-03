#pragma once
#include "KebabEngine.h"
#include <Windows.h>


namespace cuc
{
	class GLContext
	{
	public:
		GLContext(HWND, U32 width, U32 height);
		~GLContext();

		void SwapContextBuffers();
		void Resize(U32 width, U32 height);

	private:
		HWND  m_hWindow;
		HDC   m_hDeviceContext;
		HGLRC m_hRenderContext;

	private:
		void CreatePixelFormat();
		void CreateContext();
	};
}
