#pragma once
#include "PublicDef.h"
#include "../../KebabD3D12/Public/Transform.h"
#include "../../KebabD3D12/Public/Quaternion.h"
#include <vector>


namespace cuc
{
	class KEBAB_API Scene
	{
	public:
		XMFLOAT3 m_camPosition;
		Quaternion m_camRotation;
		std::vector<Transform> objTransforms;
	};
}