#include "PCH.h"
#include "InputManager.h"


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

U32 InputManager::GetMouseDeltaX()
{
	return m_mouseDeltaX;
}

U32 InputManager::GetMouseDeltaY()
{
	return m_mouseDeltaY;
}

U32 InputManager::GetMousePosX()
{
	return m_mousePosX;
}

U32 InputManager::GetMousePosY()
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

void InputManager::HandleMouseMove(U32 newPosX, U32 newPosY)
{
	m_mouseDeltaX = newPosX - m_mousePosX;
	m_mouseDeltaY = newPosY - m_mousePosY;

	m_mousePosX = newPosX;
	m_mousePosY = newPosY;
}