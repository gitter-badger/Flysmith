#pragma once
#include "PublicDef.h"
#include "../../KebabD3D12/Public/Transform.h"
#include <vector>


namespace cuc
{
	class KEBAB_API Scene
	{
	public:
		XMFLOAT3 m_camPosition;
		XMFLOAT4 m_camRotation;
		std::vector<Transform> objTransforms;
	};
}