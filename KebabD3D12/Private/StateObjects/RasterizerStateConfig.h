#pragma once
#include "PCH.h"


enum class FillMode
{
	// Fill the triangles formed by the vertices.
	SOLID			= D3D12_FILL_MODE_SOLID,
	// Draw lines connecting the vertices.
	WIREFRAME		= D3D12_FILL_MODE_WIREFRAME
};

enum class CullMode
{
	// Always draw all triangles.
	NONE		= D3D12_CULL_MODE_NONE,
	// Do not draw triangles that are front-facing.
	FRONT		= D3D12_CULL_MODE_FRONT,
	// Do not draw triangles that are back-facing.
	BACK		= D3D12_CULL_MODE_BACK
};


class RasterizerStateConfig
{
public:
	RasterizerStateConfig(FillMode fillMode = FillMode::SOLID, CullMode cullMode = CullMode::BACK);
	const D3D12_RASTERIZER_DESC& Get() const;

private:
	D3D12_RASTERIZER_DESC m_desc;
};

const RasterizerStateConfig kDefaultRasterizerState;