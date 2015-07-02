#pragma once
#include "KebabEngine.h"
#include <Windows.h>
#include <string>
#include <map>


namespace cuc
{
	class GLContext;

	class Window
	{
	public:
		Window(HINSTANCE, U32 width = 800, U32 height = 600, const std::wstring& caption = L"Window", bool bBorderless = false);
		~Window();

		void Resize(U32 width, U32 height);

		// Always call ShouldClose() after running the message loop.
		void RunMessageLoop();
		const bool ShouldClose() const;

		GLContext* GetContext() const;

	private:
		HWND m_hWnd;
		DWORD m_windowStyle;
		int x;
		int y;
		bool m_bShouldClose;

		GLContext* m_pContext;

	private:
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		static std::map<HWND, Window*> s_windows;

		static void HandleWindowEvent(HWND, UINT, WPARAM, LPARAM);
		static void HandleMouseEvent(UINT, WPARAM, LPARAM);
		static void HandleKeyboardEvent(UINT, WPARAM, LPARAM);
	};
}
