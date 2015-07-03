#include "Window.h"
#include "GLContext.h"
#include <Windows.h>
using namespace cuc;


std::map<HWND, Window*> Window::s_windows = {};


Window::Window(HINSTANCE hInstance, U32 width, U32 height, const std::wstring& caption, bool bBorderless)
	: m_hWnd(0)
	, m_bShouldClose(false)
{
	if (bBorderless)
	{
		m_windowStyle = WS_OVERLAPPED | WS_POPUP;
		x = 0;
		y = 0;
	}
	else
	{
		m_windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		x = static_cast<U32>(CW_USEDEFAULT);
		y = static_cast<U32>(CW_USEDEFAULT);
	}

	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = &Window::WndProc;
	wc.cbClsExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"KebabWindow";

	RegisterClassEx(&wc);

	RECT rect;
	rect.top = rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	AdjustWindowRectEx(&rect, m_windowStyle, false, 0);

	m_hWnd = CreateWindowEx(
		NULL,
		wc.lpszClassName,
		caption.c_str(),
		m_windowStyle,
		x,
		y,
		width,
		height,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	GetClientRect(m_hWnd, &rect);

	s_windows[m_hWnd] = this;
	m_pContext = new GLContext(m_hWnd, width, height);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
}

Window::~Window()
{
	s_windows.erase(m_hWnd);
	delete m_pContext;
}

void Window::HandleWindowEvent(HWND hwnd, UINT msg, WPARAM, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		// CreateWindowEx sends WM_CREATE only in Debug builds for some reason, even though MSDN
		// says it sends only WM_NCCREATE, WM_NCCALCSIZE, and WM_CREATE.
		// No window exists prior to its... creation.
		if (!s_windows.size())
			return;
		s_windows[hwnd]->Resize(LOWORD(lParam), HIWORD(lParam));
		break;
	}
}

void Window::HandleMouseEvent(UINT msg, WPARAM, LPARAM)
{
	switch (msg)
	{
	case WM_LBUTTONUP:
		//g_eventManager.PostEvent(new MouseLButtonUp(wParam, lParam));
		break;

	case WM_LBUTTONDOWN:
		//g_eventManager.PostEvent(new MouseLButtonDown(wParam, lParam));
		break;

	case WM_MBUTTONUP:
		//g_eventManager.PostEvent(new MouseMButtonUp(wParam, lParam));
		break;

	case WM_MBUTTONDOWN:
		//g_eventManager.PostEvent(new MouseMButtonDown(wParam, lParam));
		break;

	case WM_RBUTTONUP:
		//g_eventManager.PostEvent(new MouseRButtonUp(wParam, lParam));
		break;

	case WM_RBUTTONDOWN:
		//g_eventManager.PostEvent(new MouseRButtonDown(wParam, lParam));
		break;

	case WM_MOUSEWHEEL:
		break;
	}
}

void Window::HandleKeyboardEvent(UINT msg, WPARAM wParam, LPARAM)
{
	switch (msg)
	{
	case WM_CHAR:
		//g_eventManager.PostEvent(new KeyChar(wParam, lParam));
		break;

	case WM_KEYUP:
		//g_eventManager.PostEvent(new KeyUp(wParam, lParam));
		break;

	case WM_KEYDOWN:
		// g_eventManager.PostEvent(new KeyDown(wParam, lParam));
		break;

	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			//g_eventManager.PostEvent({ Event::EngineEvents::EXIT });
			//return 0;
		}
		break;
	}
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		return 0;

	case WM_DESTROY:
	case WM_SIZE:
		HandleWindowEvent(hwnd, msg, wParam, lParam);
		break;

	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MOUSEWHEEL:
		HandleMouseEvent(msg, wParam, lParam);
		break;

	case WM_CHAR:
	case WM_KEYUP:
	case WM_KEYDOWN:
	case WM_SYSCOMMAND:
		HandleKeyboardEvent(msg, wParam, lParam);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

const bool Window::ShouldClose() const
{
	return m_bShouldClose;
}

GLContext* Window::GetContext() const
{
	return m_pContext;
}

void Window::Resize(U32 width, U32 height)
{
	m_pContext->Resize(width, height);
}

void Window::RunMessageLoop()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			m_bShouldClose = true;
			return;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}