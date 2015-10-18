#include "PCH.h"
#include "ResourceCache.h"



ResourceCache::ResourceCache() : m_pDevice(nullptr)
{
}

void ResourceCache::Init(ID3D12Device* pDevice)
{
	assert(pDevice != nullptr);
	m_pDevice = pDevice;
}

ResourceHandle ResourceCache::AddMesh(const VertexArray& verts, const IndexArray& indices)
{
	Mesh mesh;
	mesh.verts = verts;
	mesh.indices = indices;
	mesh.Init(m_pDevice);
	m_meshes.push_back(std::move(mesh));
	return m_meshes.size() - 1;
}

void ResourceCache::UpdateMesh(ResourceHandle meshHandle, const VertexArray& verts, const IndexArray& indices)
{
	assert(ExistsMesh(meshHandle));
	m_meshes[meshHandle].verts = verts;
	m_meshes[meshHandle].indices = indices;
	m_meshes[meshHandle].Init(m_pDevice);
}

bool ResourceCache::ExistsMesh(ResourceHandle handle)
{
	return m_meshes.size() > handle;
}

Mesh& ResourceCache::GetMesh(ResourceHandle handle)
{
	assert(ExistsMesh(handle));
	return m_meshes[handle];
}

ResourceHandle ResourceCache::AddShader(ShaderType type, const wchar_t* path)
{
	m_shaders.push_back(ShaderProgram::GetCompiledShader(type, path));
	return m_shaders.size() - 1;
}

bool ResourceCache::ExistsShader(ResourceHandle handle) const
{
	return m_shaders.size() > handle;
}

const D3D12_SHADER_BYTECODE* ResourceCache::GetShader(ResourceHandle handle) const
{
	assert(ExistsShader(handle));
	return &m_shaders[handle];
}
