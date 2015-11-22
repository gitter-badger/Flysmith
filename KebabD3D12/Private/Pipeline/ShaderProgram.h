#pragma once
#include "../../KebabCommon/ShaderTypes.h"


namespace ShaderProgram
{
	D3D12_SHADER_BYTECODE GetCompiledShader(ShaderType,
		const wchar_t* path,
		const char* entryPoint = "main",
		const char* shaderModel = "5_1",
		D3D_SHADER_MACRO* pDefines = nullptr,
		bool bAllowIncludes = true,
		UINT flags = 0);

	D3D12_SHADER_BYTECODE GetCompiledShader(ShaderType,
		const void* pData,
		const size_t dataSize,
		const char* sourceName = nullptr,
		const char* entryPoint = "main",
		const char* shaderModel = "5_1",
		D3D_SHADER_MACRO* pDefines = nullptr,
		bool bAllowIncludes = true,
		UINT flags = 0);
};