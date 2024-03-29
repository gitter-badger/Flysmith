#pragma once
#include "PCH.h"
#include "PublicDef.h"


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
	
	void EnableCursor();
	void DisableCursor();

	void Clip();
	void Unclip();

private:
	HWND m_hWnd;
	DWORD m_windowStyle;
	U32 m_positionX;
	U32 m_positionY;
	U32 m_width;
	U32 m_height;
	bool m_bShouldClose;

	bool m_bCursorDisabled;

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static std::map<HWND, Window*> s_windows;
};