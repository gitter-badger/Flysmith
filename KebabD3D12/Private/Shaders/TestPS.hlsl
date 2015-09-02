#include "TestHeader.hlsli"


cbuffer color : register(b0)
{
	uint r;
	uint g;
	uint b;
	uint a;
};

float4 main(VS_OUT vsin) : SV_TARGET
{
	return float4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}