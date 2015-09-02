#include "PCH.h"
#include "InputManager.h"
#include "Application\Window.h"


bool InputManager::s_bInstantiated = false;

InputManager::InputManager()
{
	assert(!s_bInstantiated);
	s_bInstantiated = true;

	for (U8 key = 32; key <= 127; key++)
		m_keyStates[key] = false;

	m_mousePosX = 0;
	m_mousePosY = 0;
	m_mouseDeltaX = 0;
	m_mouseDeltaY = 0;

	m_bLMBDown = false;
	m_bRMBDown = false;
	m_bCursorDisabled = false;
}

InputManager::~InputManager()
{
	s_bInstantiated = false;
}

bool InputManager::IsKeyDown(U8 key)
{
	return m_keyStates[key];
}

void InputManager::SetKeyUp(U8 key)
{
	m_keyStates[key] = false;
}

void InputManager::SetKeyDown(U8 key)
{
	m_keyStates[key] = true;
}

I32 InputManager::GetMouseDeltaX()
{
	return m_mouseDeltaX;
}

I32 InputManager::GetMouseDeltaY()
{
	return m_mouseDeltaY;
}

I32 InputManager::GetMousePosX()
{
	return m_mousePosX;
}

I32 InputManager::GetMousePosY()
{
	return m_mousePosY;
}

bool InputManager::IsRMBDown()
{
	return m_bRMBDown;
}

bool InputManager::IsLMBDown()
{
	return m_bLMBDown;
}

void InputManager::SetRMBUp()
{
	m_bRMBDown = false;
}

void InputManager::SetRMBDown()
{
	m_bRMBDown = true;
}

void InputManager::SetLMBUp()
{
	m_bLMBDown = false;
}

void InputManager::SetLMBDown()
{
	m_bLMBDown = true;
}

void InputManager::HandleMouseMove(I32 newPosX, I32 newPosY)
{
	m_mouseDeltaX = newPosX - m_mousePosX;
	m_mouseDeltaY = newPosY - m_mousePosY;

	m_mousePosX = newPosX;
	m_mousePosY = newPosY;
	
	// TODO: BORKED
	if (m_bCursorDisabled)
	{
		/*POINT center;
		center.x = m_pWindow->GetWidth() / 2;
		center.y = m_pWindow->GetHeight() / 2;

		m_mousePosX = center.x;
		m_mousePosY = center.y;
		
		ClientToScreen(m_pWindow->GetHandle(), &center);
		SetCursorPos(center.x, center.y);*/
	}
}

void InputManager::Reset()
{
	m_mouseDeltaX = 0;
	m_mouseDeltaY = 0;
}

void InputManager::EnableCursor()
{
	ShowCursor(TRUE);
	if (m_bCursorDisabled)
	{
		ClientToScreen(m_hwnd, &m_mouseRestore);
		SetCursorPos(m_mouseRestore.x, m_mouseRestore.y);

		m_mousePosX = m_mouseRestore.x;
		m_mousePosY = m_mouseRestore.y;
	}

	m_bCursorDisabled = false;
}

void InputManager::DisableCursor()
{
	ShowCursor(FALSE);

	m_mouseRestore.x = m_mousePosX;
	m_mouseRestore.y = m_mousePosY;

	m_bCursorDisabled = true;
}

void InputManager::Init(Window* pWindow)
{
	m_pWindow = pWindow;
	m_hwnd = pWindow->GetHandle();
}