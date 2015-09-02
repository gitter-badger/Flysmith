#pragma once
#include "PublicDef.h"
#include <vector>
#include "../../KebabD3D12/Public/Component.h"


class KEBAB_API Entity
{
public:
	std::vector<Component> m_components;
	Entity();
	I32 GetId();

protected:
	I32 m_id;

private:
	static I32 s_nextId;
};