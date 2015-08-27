#pragma once


namespace cuc
{
	struct RenderItem
	{
		RenderItem(ResourceHandle mesh_, ResourceHandle vertShader_, ResourceHandle pixelShader_)
			: mesh(mesh_), vertShader(vertShader_), pixelShader(pixelShader_) {}

		ResourceHandle mesh;
		ResourceHandle vertShader;
		ResourceHandle pixelShader;
	};
}