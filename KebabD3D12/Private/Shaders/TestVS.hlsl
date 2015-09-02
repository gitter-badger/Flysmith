#include "TestHeader.hlsli"


cbuffer cbPerDraw : register(b1)
{
	float4x4 viewProjMat;
};

cbuffer cbPerObject : register(b2)
{
	float4x4 worldMat;
};

VS_OUT main(VS_IN vsin)
{
	float4x4 identityMat = { 1, 0, 0, 0,
							 0, 1, 0, 0,
							 0, 0, 1, 0,
							 0, 0, 0, 1 };
	
	VS_OUT output;
	output.pos = mul(float4(vsin.pos, 1.0f), mul(worldMat, viewProjMat));
	output.normal = vsin.normal;

	return output;
}