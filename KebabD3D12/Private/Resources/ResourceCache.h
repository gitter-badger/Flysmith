#pragma once
#include <vector>
#include "Mesh.h"
#include "Pipeline\ShaderProgram.h"


class ResourceCache
{
public:
	ResourceCache();
	void Init(ID3D12Device*);

	// TODO: Handle all resource types uniformly
	ResourceHandle AddMesh(const VertexArray&, const IndexArray&);
	void UpdateMesh(ResourceHandle meshHandle, const VertexArray&, const IndexArray&);
	bool ExistsMesh(ResourceHandle);
	Mesh& GetMesh(ResourceHandle);

	//ResourceHandle AddShader(ShaderType, const void* pData, size_t dataSize, const char* sourceName = nullptr);
	ResourceHandle AddShader(ShaderType, const wchar_t* path);
	bool ExistsShader(ResourceHandle) const;
	const D3D12_SHADER_BYTECODE* GetShader(ResourceHandle) const;

private:
	std::vector<Mesh> m_meshes;
	std::vector<D3D12_SHADER_BYTECODE> m_shaders;
	ID3D12Device* m_pDevice;
};