#pragma once
#include "PCH.h"
#include "PublicDef.h"


namespace cuc
{
	class Window
	{
	public:
		Window(HINSTANCE, U32 width = 800, U32 height = 600, const std::wstring& caption = L"Window", bool bBorderless = false);
		~Window();

		// Always call ShouldClose() after running the message loop.
		void RunMessageLoop();
		const bool ShouldClose() const;

		const HWND GetHandle() const;
		const U32  GetWidth() const;
		const U32  GetHeight() const;

	private:
		HWND m_hWnd;
		DWORD m_windowStyle;
		U32 m_positionX;
		U32 m_positionY;
		U32 m_width;
		U32 m_height;
		bool m_bShouldClose;

	private:
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		static std::map<HWND, Window*> s_windows;

		static void HandleWindowEvent(HWND, UINT, WPARAM, LPARAM);
		static void HandleMouseEvent(UINT, WPARAM, LPARAM);
		static void HandleKeyboardEvent(UINT, WPARAM, LPARAM);
	};
}
