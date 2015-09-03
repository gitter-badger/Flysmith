#pragma once
#include "PublicDef.h"
#include "Component.h"
#include <vector>


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