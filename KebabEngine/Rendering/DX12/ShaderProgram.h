#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_DX12
#include <d3d12.h>


namespace cuc
{
#pragma push_macro("DOMAIN")
#undef DOMAIN
	enum class ShaderType
	{
		COMPUTE,
		VERTEX,
		HULL,
		DOMAIN,
		GEOMETRY,
		PIXEL
	};
#pragma pop_macro("DOMAIN")

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
	};
}

#endif