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
	// WTF is going on 
	float4x4 wvp = mul(worldMat, viewProjMat);
	output.pos = mul(float4(vsin.pos, 1.0f), wvp);
	//output.pos = mul(float4(vsin.pos, 1.0f), viewProjMat * worldMat);
	//output.pos = mul(float4(vsin.pos, 1.0f), viewProjMat);
	output.color = vsin.color;

	return output;
}