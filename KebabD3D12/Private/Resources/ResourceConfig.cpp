#include "PCH.h"
#include "ResourceConfig.h"


cuc::ResourceConfig::ResourceConfig(ResourceType type, U64 width, U32 height, DXGI_FORMAT format, TextureLayout layout, ResourceFlag flags)
{
	m_desc = new D3D12_RESOURCE_DESC;

	m_desc->Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(type);
	m_desc->Width = width;
	m_desc->Height = height;
	m_desc->Format = format;
	m_desc->Flags = static_cast<D3D12_RESOURCE_FLAGS>(flags);
	m_desc->Layout = static_cast<D3D12_TEXTURE_LAYOUT>(layout);

	m_desc->Alignment = 0;
	m_desc->DepthOrArraySize = 1;
	m_desc->MipLevels = 1;
	m_desc->SampleDesc.Count = 1;
	m_desc->SampleDesc.Quality = 0;
}

cuc::ResourceConfig::~ResourceConfig()
{
	delete m_desc;
}

void cuc::ResourceConfig::SetType(ResourceType type)
{
	m_desc->Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(type);
}

void cuc::ResourceConfig::SetWidth(U64 width)
{
	m_desc->Width = width;
}

void cuc::ResourceConfig::SetHeight(U32 height)
{
	m_desc->Height = height;
}

void cuc::ResourceConfig::SetFormat(DXGI_FORMAT format)
{
	m_desc->Format = format;
}

void cuc::ResourceConfig::SetFlags(ResourceFlag flags)
{
	m_desc->Flags = static_cast<D3D12_RESOURCE_FLAGS>(flags);
}

void cuc::ResourceConfig::SetLayout(TextureLayout layout)
{
	m_desc->Layout = static_cast<D3D12_TEXTURE_LAYOUT>(layout);
}

const D3D12_RESOURCE_DESC& cuc::ResourceConfig::Get() const
{
	return *m_desc;
}
