#pragma once
#include "PCH.h"


namespace cuc
{
	// Added _SHADER suffix because of a preprocessor define DOMAIN in math.h
	enum ShaderType
	{
		COMPUTE_SHADER,
		VERTEX_SHADER,
		HULL_SHADER,
		DOMAIN_SHADER,
		GEOMETRY_SHADER,
		PIXEL_SHADER
	};

	class ShaderProgram
	{
	public:
		static D3D12_SHADER_BYTECODE GetCompiledShader(const ShaderType,
													   const wchar_t* path, 
													   const char* entryPoint = "main",
													   const char* shaderModel = "5_1",
													   const D3D_SHADER_MACRO* pDefines = nullptr,
													   const bool bAllowIncludes = true,
													   UINT flags = 0);

		static D3D12_SHADER_BYTECODE GetCompiledShader(const ShaderType,
													   const void* pData,
													   const size_t dataSize,
													   const char* sourceName = nullptr,
													   const char* entryPoint = "main",
													   const char* shaderModel = "5_1",
													   const D3D_SHADER_MACRO* pDefines = nullptr,
													   const bool bAllowIncludes = true,
													   UINT flags = 0);
	};
}