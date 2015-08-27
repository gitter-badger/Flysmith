#pragma once
#include "PublicDef.h"
#include <vector>
#include "Mesh.h"
#include "Pipeline\ShaderProgram.h"


class ResourceCache
{
public:
	ResourceCache();

	// TODO: Handle all resource types uniformly
	ResourceHandle AddMesh(ID3D12Device*, const std::vector<Vertex>& verts, const std::vector<U32>& indices);
	bool ExistsMesh(ResourceHandle);
	Mesh& GetMesh(ResourceHandle);

	//ResourceHandle AddShader(ShaderType, const void* pData, size_t dataSize, const char* sourceName = nullptr);
	ResourceHandle AddShader(ShaderType, const wchar_t* path);
	bool ExistsShader(ResourceHandle);
	D3D12_SHADER_BYTECODE& GetShader(ResourceHandle);


private:
	std::vector<Mesh> m_meshes;
	std::vector<D3D12_SHADER_BYTECODE> m_shaders;
};