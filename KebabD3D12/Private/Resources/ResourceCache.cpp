#include "PCH.h"
#include "ResourceCache.h"
using namespace cuc;


ResourceCache::ResourceCache()
{
}

ResourceHandle ResourceCache::AddMesh(const Mesh& mesh)
{
	m_meshes.push_back(mesh);
	return m_meshes.size() - 1;
}

void ResourceCache::UpdateMesh(ResourceHandle handle, const Mesh& updatedMesh)
{
	assert(ExistsMesh(handle));
	m_meshes[handle] = updatedMesh;
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
