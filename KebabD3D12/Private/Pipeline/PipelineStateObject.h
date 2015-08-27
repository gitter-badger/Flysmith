#pragma once
#include "PCH.h"
#include "StateObjects\RasterizerStateConfig.h"
#include "StateObjects\BlendStateConfig.h"
#include "ShaderProgram.h"


class PipelineStateObject
{
public:
	PipelineStateObject();
	PipelineStateObject(ID3D12Device*, const D3D12_GRAPHICS_PIPELINE_STATE_DESC&);
	PipelineStateObject(ID3D12Device*, const D3D12_INPUT_ELEMENT_DESC*, const U32 numLayoutElements, ID3D12RootSignature* = nullptr,
		const BlendStateConfig* = nullptr, const RasterizerStateConfig* = nullptr, const D3D12_SHADER_BYTECODE* VS = nullptr,
		const D3D12_SHADER_BYTECODE* PS = nullptr, const D3D12_SHADER_BYTECODE* GS = nullptr, const D3D12_SHADER_BYTECODE* HS = nullptr,
		const D3D12_SHADER_BYTECODE* DS = nullptr, const D3D12_PRIMITIVE_TOPOLOGY_TYPE = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	~PipelineStateObject();

	void Init(ID3D12Device*, const D3D12_GRAPHICS_PIPELINE_STATE_DESC&);
	void Init(ID3D12Device*,
		const D3D12_INPUT_ELEMENT_DESC*,
		const U32 numLayoutElements,
		ID3D12RootSignature* = nullptr,            // All used shaders must have a matching root signature if no root signature is passed in.
		const BlendStateConfig* = nullptr,         // Leave null to use the default blend state configuration
		const RasterizerStateConfig* = nullptr,    // Leave null to use the default rasterizer state configuration
		const D3D12_SHADER_BYTECODE* VS = nullptr,
		const D3D12_SHADER_BYTECODE* PS = nullptr,
		const D3D12_SHADER_BYTECODE* GS = nullptr,
		const D3D12_SHADER_BYTECODE* HS = nullptr,
		const D3D12_SHADER_BYTECODE* DS = nullptr,
		const D3D12_PRIMITIVE_TOPOLOGY_TYPE = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

	// Use the following only when changing a single setting, since the PSO will be recreated on each change.
	// Use the Init() methods when changing multiple settings.
	// ------------------------------------------------------------------------------------------------------
	void SetShader(ID3D12Device*, const ShaderType, D3D12_SHADER_BYTECODE*);
	void SetShader(ID3D12Device*, const ShaderType, ID3DBlob*);

	ID3D12PipelineState* Get() const;

private:
	ID3D12PipelineState* m_pState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_description;
};