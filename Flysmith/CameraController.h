#pragma once
#include <string>
#include "EventListener.h"
#include "InputEvents.h"
#include "GameStateEvents.h"
#include <memory>
#include "../../KebabD3D12/Public/Transform.h"
using namespace cuc;


class CameraController : public EventListener
{
public:
	CameraController(Transform* camTransform)
		: m_camTransform(camTransform)
	{
		RegisterForEvent(KeyboardEvent::KeyUpId);
		RegisterForEvent(KeyboardEvent::KeyDownId);
		RegisterForEvent("Tick"_HASH);
	}

	Transform* m_camTransform;

	void HandleEvent(const Event& ev)
	{
		float velocity = 1.0f;

		switch (ev.type)
		{
		case "Tick"_HASH:
		{
			float disp = velocity * ev[0].GetFloat();
			if (bPressedW)
				m_camTransform->TranslateZ(disp);
			if (bPressedS)
				m_camTransform->TranslateZ(-disp);
			if (bPressedA)
				m_camTransform->TranslateX(-disp);
			if (bPressedD)
				m_camTransform->TranslateX(disp);
			break;
		}
		case KeyboardEvent::KeyUpId:
			if (ev[KeyboardEvent::U32_KEY_CODE].GetUnsignedInt() == 'W')
				bPressedW = false;
			if (ev[KeyboardEvent::U32_KEY_CODE].GetUnsignedInt() == 'A')
				bPressedA = false;
			if (ev[KeyboardEvent::U32_KEY_CODE].GetUnsignedInt() == 'S')
				bPressedS = false;
			if (ev[KeyboardEvent::U32_KEY_CODE].GetUnsignedInt() == 'D')
				bPressedD = false;
			break;
		case KeyboardEvent::KeyDownId:
			if (ev[KeyboardEvent::U32_KEY_CODE].GetUnsignedInt() == 'W')
				bPressedW = true;
			if (ev[KeyboardEvent::U32_KEY_CODE].GetUnsignedInt() == 'A')
				bPressedA = true;
			if (ev[KeyboardEvent::U32_KEY_CODE].GetUnsignedInt() == 'S')
				bPressedS = true;
			if (ev[KeyboardEvent::U32_KEY_CODE].GetUnsignedInt() == 'D')
				bPressedD = true;
			break;
		}
	}

private:
	bool bPressedW = false,
		 bPressedS = false,
		 bPressedA = false,
		 bPressedD = false;
};