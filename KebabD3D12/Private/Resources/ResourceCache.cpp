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
