#pragma once
#include "PublicDef.h"
#include <vector>
#include "Mesh.h"


namespace cuc
{
	class ResourceCache
	{
	public:
		ResourceCache();
		
		ResourceHandle AddMesh(ID3D12Device*, const std::vector<Vertex>& verts, const std::vector<U32>& indices);
		bool ExistsMesh(ResourceHandle);
		Mesh& GetMesh(ResourceHandle);

	private:
		std::vector<Mesh> m_meshes;
	};
}