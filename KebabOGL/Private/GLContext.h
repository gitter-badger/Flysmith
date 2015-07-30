#pragma once
#include "PCH.h"
#include "PublicDef.h"


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
		
		static bool s_bLoadedFunctions;
		void LoadFunctions();

		void CreatePixelFormat();
		void CreateContext();
		void DestroyContext();
	};
}