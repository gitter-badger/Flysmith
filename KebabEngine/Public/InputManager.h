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

public:
	InputManager();
	~InputManager();

	bool IsKeyDown(U8 key);
	bool IsRMBDown();
	bool IsLMBDown();
	U32 GetMouseDeltaX();
	U32 GetMouseDeltaY();
	U32 GetMousePosX();
	U32 GetMousePosY();

private:
	static bool s_bInstantiated;
	std::map<U8, bool> m_keyStates;
	U32 m_mousePosX;
	U32 m_mousePosY;
	U32 m_mouseDeltaX;
	U32 m_mouseDeltaY;
	bool m_bLMBDown;
	bool m_bRMBDown;

	void SetKeyUp(U8 key);
	void SetKeyDown(U8 key);
	void HandleMouseMove(U32 newPosX, U32 newPosY);
	void SetRMBUp();
	void SetRMBDown();
	void SetLMBUp();
	void SetLMBDown();
};

extern InputManager g_inputManager;