#pragma once
#include <map>
#include "PublicDef.h"


// Mainly used for continuous input
// i.e. Used by systems that need to check input state each frame
// TODO: Inline
// TODO: Lock cursor inside window
class KEBAB_API InputManager
{
	friend class Window;
	friend class Application;

public:
	InputManager();
	~InputManager();

	bool IsKeyDown(U8 key);
	bool IsRMBDown();
	bool IsLMBDown();
	I32 GetMouseDeltaX();
	I32 GetMouseDeltaY();
	I32 GetMousePosX();
	I32 GetMousePosY();

private:
	static bool s_bInstantiated;
	std::map<U8, bool> m_keyStates;
	I32 m_mousePosX;
	I32 m_mousePosY;
	I32 m_mouseDeltaX;
	I32 m_mouseDeltaY;
	bool m_bLMBDown;
	bool m_bRMBDown;

	void SetKeyUp(U8 key);
	void SetKeyDown(U8 key);
	void HandleMouseMove(I32 newPosX, I32 newPosY);
	void SetRMBUp();
	void SetRMBDown();
	void SetLMBUp();
	void SetLMBDown();

	void Reset();
};

extern KEBAB_API InputManager g_inputManager;