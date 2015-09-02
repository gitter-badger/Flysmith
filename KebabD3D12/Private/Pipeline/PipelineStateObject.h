#pragma once
#include "PCH.h"
#include "StateObjects\RasterizerStateConfig.h"
#include "StateObjects\BlendStateConfig.h"
#include "ShaderProgram.h"


struct ShaderPipeline
{
	ShaderPipeline(const D3D12_SHADER_BYTECODE* VS, const D3D12_SHADER_BYTECODE* PS, const D3D12_SHADER_BYTECODE* GS = nullptr, 
				   const D3D12_SHADER_BYTECODE* HS = nullptr, const D3D12_SHADER_BYTECODE* DS = nullptr);
	const D3D12_SHADER_BYTECODE* VS;
	const D3D12_SHADER_BYTECODE* PS;
	const D3D12_SHADER_BYTECODE* GS;
	const D3D12_SHADER_BYTECODE* HS;
	const D3D12_SHADER_BYTECODE* DS;
};

enum class PrimitiveTopologyType
{
	UNDEFINED,
	POINT,
	LINE,
	TRIANGLE,
	PATCH
};

class PipelineStateObject
{
public:
	PipelineStateObject();
	PipelineStateObject(PipelineStateObject&);
	PipelineStateObject& operator=(PipelineStateObject&);
	PipelineStateObject(PipelineStateObject&&);
	PipelineStateObject& operator=(PipelineStateObject&&);
	~PipelineStateObject();
	
	void Init(ID3D12Device*,
			  const D3D12_INPUT_ELEMENT_DESC*,
			  const U32 numLayoutElements,
			  const ShaderPipeline&,
			  ID3D12RootSignature* = nullptr,            // All used shaders must have a matching root signature if no root signature is passed in.
			  const RasterizerStateConfig* = nullptr,    // Leave null to use the default rasterizer state configuration
			  const BlendStateConfig* = nullptr,         // Leave null to use the default blend state configuration
			  PrimitiveTopologyType = PrimitiveTopologyType::TRIANGLE);

	friend bool operator==(PipelineStateObject&, PipelineStateObject&);

	FillMode    GetFillMode() const;
	CullMode    GetCullMode() const;
	const void* GetVertexShader() const;
	const void* GetPixelShader() const;

	ID3D12PipelineState* Get() const;

private:
	bool m_bInitialized;
	ID3D12PipelineState* m_pState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_description;
};
