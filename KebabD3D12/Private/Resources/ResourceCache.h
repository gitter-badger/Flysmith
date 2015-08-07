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
		
		ResourceHandle AddMesh(const Mesh&);
		void UpdateMesh(ResourceHandle, const Mesh&);
		bool ExistsMesh(ResourceHandle);
		Mesh& GetMesh(ResourceHandle);

	private:
		std::vector<Mesh> m_meshes;
	};
}