#pragma once
#include <string>
#include "EventListener.h"
#include "InputEvents.h"
#include "GameStateEvents.h"
#include <memory>
#include "../../KebabD3D12/Public/Transform.h"
#include "StringHash.h"
#include <map>
using namespace cuc;


class CameraController : public EventListener
{
public:
	CameraController(XMFLOAT3* camPosition, XMFLOAT4* camRotation);
	void Update(F32 dt);
	void HandleEvent(const Event& ev);

private:
	void Init();
	void Move(CXMVECTOR);
	void SetFocalPointAndPosition(CXMVECTOR focalPoint, CXMVECTOR position);
	void SetAxis(CXMVECTOR lookAt, CXMVECTOR up, CXMVECTOR right);
	XMVECTOR GetRight();
	XMVECTOR GetLookAt();

	XMFLOAT3* m_camPosition;
	XMFLOAT4* m_camRotation;
	std::map<char, bool> m_bPressed;
	std::vector<char> m_keys;
};