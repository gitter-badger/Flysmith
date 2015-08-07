#include "PCH.h"
#include "ShaderProgram.h"
#include <d3dcompiler.h>
using namespace cuc;


std::map<ShaderType, const char*> shaderTypeStringNames = {
	{ ShaderType::COMPUTE_SHADER, "cs" },
	{ ShaderType::VERTEX_SHADER, "vs" },
	{ ShaderType::HULL_SHADER, "hs" },
	{ ShaderType::DOMAIN_SHADER, "ds" },
	{ ShaderType::GEOMETRY_SHADER, "gs" },
	{ ShaderType::PIXEL_SHADER, "ps" }
};

std::unordered_set<const wchar_t*> loadedShaders;


D3D12_SHADER_BYTECODE ShaderProgram::GetCompiledShader(const ShaderType type, const wchar_t* path, const char* entryPoint, const char* shaderModel,
													   const D3D_SHADER_MACRO* pDefines, const bool bAllowIncludes, UINT flags)
{
	assert(path != nullptr);
	assert(loadedShaders.find(path) == loadedShaders.end());
	loadedShaders.insert(path);

	ID3DInclude* pIncludeFlag = nullptr;
	if (bAllowIncludes)
		pIncludeFlag = D3D_COMPILE_STANDARD_FILE_INCLUDE;

	// TODO: Remove row major flag
	flags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif

#pragma warning(push)
#pragma warning(disable:4996)
	// 2 bytes for shader type
	// 4 bytes for the shader model
	// 1 byte for the null terminator
	char shaderModelName[7];
	strcpy(shaderModelName, shaderTypeStringNames[type]);
	strcat(shaderModelName, "_");
	strcat(shaderModelName, shaderModel);
#pragma warning(pop)

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(path,
									pDefines,
									pIncludeFlag,
									entryPoint,
									shaderModelName,
									flags,
									0,      // Effect file flags
									&shaderBlob,
									&errorBlob);
	assert(SUCCEEDED(hr));
	assert(!errorBlob);

	if (errorBlob)
	{
		shaderBlob->Release();
		errorBlob->Release();
	}
	
	D3D12_SHADER_BYTECODE result;
	result.BytecodeLength = shaderBlob->GetBufferSize();
	result.pShaderBytecode = reinterpret_cast<BYTE*>(shaderBlob->GetBufferPointer());
	return result;
}

D3D12_SHADER_BYTECODE cuc::ShaderProgram::GetCompiledShader(const ShaderType type, const void* pData, const size_t dataSize, const char* sourceName, 
															const char* entryPoint, const char* shaderModel, 
															const D3D_SHADER_MACRO* pDefines, const bool bAllowIncludes, UINT flags)
{
	ID3DInclude* pIncludeFlag = nullptr;
	if (bAllowIncludes)
		pIncludeFlag = D3D_COMPILE_STANDARD_FILE_INCLUDE;

	// TODO: Remove row major flag
	flags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR; 
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif

#pragma warning(push)
#pragma warning(disable:4996)
	// 2 bytes for shader type
	// 4 bytes for the shader model
	// 1 byte for the null terminator
	char shaderModelName[7];
	strcpy(shaderModelName, shaderTypeStringNames[type]);
	strcat(shaderModelName, "_");
	strcat(shaderModelName, shaderModel);
#pragma warning(pop)

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompile(pData,
							dataSize,
							sourceName,
							pDefines,
							pIncludeFlag,
							entryPoint,
							shaderModelName,
							flags,
							0,      // Effect file flags
							&shaderBlob,
							&errorBlob);
	assert(SUCCEEDED(hr));
	assert(!errorBlob);

	if (errorBlob)
	{
		shaderBlob->Release();
		errorBlob->Release();
	}

	D3D12_SHADER_BYTECODE result;
	result.BytecodeLength = shaderBlob->GetBufferSize();
	result.pShaderBytecode = reinterpret_cast<BYTE*>(shaderBlob->GetBufferPointer());
	return result;
}
