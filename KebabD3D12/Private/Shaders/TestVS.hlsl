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
	VS_OUT output;
	float4x4 worldViewProjMat = viewProjMat * worldMat;
	//output.pos = mul(float4(vsin.pos, 1.0f), worldViewProjMat);
	output.pos = float4(vsin.pos, 1.0f);
	output.color = vsin.color;

	return output;
}