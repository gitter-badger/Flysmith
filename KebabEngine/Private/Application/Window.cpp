#include "PCH.h"
#include "Window.h"
#include "InputEvents.h"
#include "WindowEvents.h"
#include "Events\EventManager.h"
using namespace cuc;


std::map<HWND, Window*> Window::s_windows = {};


Window::Window(HINSTANCE hInstance, U32 width, U32 height, const std::wstring& caption, bool bBorderless)
	: m_hWnd(0)
	, m_bShouldClose(false)
	, m_width(width)
	, m_height(height)
{
	if (bBorderless)
	{
		m_windowStyle = WS_OVERLAPPED | WS_POPUP;
		m_positionX = 0;
		m_positionY = 0;
	}
	else
	{
		m_windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		m_positionX = static_cast<U32>(CW_USEDEFAULT);
		m_positionY = static_cast<U32>(CW_USEDEFAULT);
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
		m_positionX,
		m_positionY,
		width,
		height,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	GetClientRect(m_hWnd, &rect);

	s_windows[m_hWnd] = this;
	
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);
}

Window::~Window()
{
	s_windows.erase(m_hWnd);
}

void Window::HandleWindowEvent(HWND hwnd, UINT msg, WPARAM, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		if (!s_windows.size())
			return;
		g_eventManager.PostEvent(WindowResizeEvent::Create(LOWORD(lParam), HIWORD(lParam)));
		s_windows[hwnd]->m_width = LOWORD(lParam);
		s_windows[hwnd]->m_height = HIWORD(lParam);
		break;
	}
}

void Window::HandleMouseEvent(UINT msg, WPARAM, LPARAM lParam)
{
	F32 xPosition = static_cast<F32>(GET_X_LPARAM(lParam));
	F32 yPosition = static_cast<F32>(GET_Y_LPARAM(lParam));

#define PostMouseEv(WindowsMessage, EventId) \
	case WindowsMessage: \
		g_eventManager.PostEvent(MouseEvent::Create(MouseEvent::EventId, xPosition, yPosition)); \
		break;

	switch (msg)
	{
		PostMouseEv(WM_LBUTTONUP,   LMouseUpId)
		PostMouseEv(WM_LBUTTONDOWN, LMouseDownId)
		PostMouseEv(WM_RBUTTONUP,   RMouseUpId)
		PostMouseEv(WM_RBUTTONDOWN, RMouseDownId)
	}
}

void Window::HandleKeyboardEvent(UINT msg, WPARAM wParam, LPARAM)
{
	switch (msg)
	{
	case WM_CHAR:
		g_eventManager.PostEvent(KeyboardEvent::Create(KeyboardEvent::KeyCharId, wParam));
		break;

	case WM_KEYUP:
		g_eventManager.PostEvent(KeyboardEvent::Create(KeyboardEvent::KeyUpId, wParam));
		break;

	case WM_KEYDOWN:
		g_eventManager.PostEvent(KeyboardEvent::Create(KeyboardEvent::KeyDownId, wParam));
		break;

	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			//g_eventManager.PostEvent({ Event::EngineEvents::EXIT });
			return;
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

const HWND Window::GetHandle() const
{
	return m_hWnd;
}

const U32 cuc::Window::GetWidth() const
{
	return m_width;
}

const U32 cuc::Window::GetHeight() const
{
	return m_height;
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