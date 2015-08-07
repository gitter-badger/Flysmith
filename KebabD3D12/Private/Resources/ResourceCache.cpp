#include "PCH.h"
#include "ResourceCache.h"
using namespace cuc;


ResourceCache::ResourceCache()
{
}

ResourceHandle ResourceCache::AddMesh(ID3D12Device* pDevice, const std::vector<Vertex>& verts, const std::vector<U32>& indices)
{
	Mesh mesh;
	mesh.verts = verts;
	mesh.indices = indices;
	mesh.Init(pDevice);
	m_meshes.push_back(std::move(mesh));
	return m_meshes.size() - 1;
}

bool cuc::ResourceCache::ExistsMesh(ResourceHandle handle)
{
	return m_meshes.size() > handle;
}

Mesh& cuc::ResourceCache::GetMesh(ResourceHandle handle)
{
	assert(ExistsMesh(handle));
	return m_meshes[handle];
}

ResourceHandle cuc::ResourceCache::AddShader(ShaderType type, const wchar_t* path)
{
	m_shaders.push_back(ShaderProgram::GetCompiledShader(type, path));
	return m_shaders.size() - 1;
}

bool cuc::ResourceCache::ExistsShader(ResourceHandle handle)
{
	return m_shaders.size() > handle;
}

D3D12_SHADER_BYTECODE& cuc::ResourceCache::GetShader(ResourceHandle handle)
{
	assert(ExistsShader(handle));
	return m_shaders[handle];
}
