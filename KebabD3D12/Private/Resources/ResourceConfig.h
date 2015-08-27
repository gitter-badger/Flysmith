#pragma once
#include "PublicDef.h"


struct D3D12_RESOURCE_DESC;

enum ResourceType
{
	//UNKNOWN  = 0,
	BUFFER = 1,
	TEXTURE1D = 2,
	TEXTURE2D = 3,
	TEXTURE3D = 4
};

enum TextureLayout
{
	UNKNOWN = 0,
	ROW_MAJOR = 1,
	UNDEFINED_SWIZZLE = 2,
	STANDARD_SWIZZLE = 3
};

enum ResourceFlag
{
	NONE = 0,
	// Allows a render target view to be created for this resource.
	// Enables transition into RENDER_TARGET state.
	// Cannot be used with 4K alignment.
	ALLOW_RENDER_TARGET = 0x1,
	// Allows a depth stencil view to be created for the resource.
	// Enables transition into DEPTH_WRITE/DEPTH_READ state.
	// Cannot be used with 4K alignment.
	ALLOW_DEPTH_STENCIL = 0x2,
	// Allows an UAV to be created for the resource.
	// Enables transition into UNORDERED_ACCESS state.
	// Cannot be used with MSAA textures.
	ALLOW_UNORDERED_ACCESS = 0x4,
	// Disallows shader resource view from being created for this resource.
	// Disables transition into NON_PIXEL_SHADER_RESOURCE or PIXEL_SHADER_RESOURCE states.
	// Must be used with ALLOW_DEPTH_STENCIL. 
	DENY_SHADER_RESOURCE = 0x8,
	// Allow cross-adapter data use. 
	// Must be used with heaps that have SHARED_CROSS_ADAPTER.
	// Cannot be used with heaps that have ALLOW_DISPLAY.
	ALLOW_CROSS_ADAPTER = 0x10,
	// Allows simultaneous access by different queues/devices/processes.
	// One writer with multiple readers, as long as the writer doesn't concurrently modify while readers access.
	// Cannot be used with BUFFER resources(they already have this property).
	// Cannot be used with MSAA textures.
	ALLOW_SIMULTANEOUS_ACCESS = 0x20
};

class ResourceConfig
{
public:
	// @width - width of the texture; in case of buffers, the buffer's size
	// @height - height of the texture; in case of buffers, leave 1 
	ResourceConfig(ResourceType, U64 width, U32 height = 1, DXGI_FORMAT = DXGI_FORMAT_UNKNOWN, TextureLayout = ROW_MAJOR, ResourceFlag = NONE);
	~ResourceConfig();

	void SetType(ResourceType);
	void SetWidth(U64);
	void SetHeight(U32);
	void SetFormat(DXGI_FORMAT);
	void SetFlags(ResourceFlag);
	void SetLayout(TextureLayout);

	const D3D12_RESOURCE_DESC& Get() const;

private:
	D3D12_RESOURCE_DESC* m_desc;
};