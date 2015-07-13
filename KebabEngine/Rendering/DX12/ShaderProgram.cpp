#include "ShaderProgram.h"
#ifdef RENDERER_DX12
#include <d3dcompiler.h>
#include <cassert>
#include <map>
#include <unordered_set>
using namespace cuc;


#pragma push_macro("DOMAIN")
#undef DOMAIN
std::map<ShaderType, const char*> shaderTypeStringNames = {
	{ ShaderType::COMPUTE, "cs" },
	{ ShaderType::VERTEX, "vs" },
	{ ShaderType::HULL, "hs" },
	{ ShaderType::DOMAIN, "ds" },
	{ ShaderType::GEOMETRY, "gs" },
	{ ShaderType::PIXEL, "ps" }
};
#pragma pop_macro("DOMAIN")

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

	flags |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#endif

	// 2 bytes for shader type
	// 4 bytes for the shader model
	// 1 byte for the null terminator
	char shaderModelName[7];
	strcpy(shaderModelName, shaderTypeStringNames[type]);
	strcat(shaderModelName, "_");
	strcat(shaderModelName, shaderModel);

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(
		path,
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
		
		errorBlob->Release();
	}
	
	D3D12_SHADER_BYTECODE result;
	result.BytecodeLength = shaderBlob->GetBufferSize();
	result.pShaderBytecode = reinterpret_cast<BYTE*>(shaderBlob->GetBufferPointer());
	return result;
}

#endif