#include "TestHeader.hlsli"


cbuffer cbPerObject : register(b0)
{
	float4x4 mvp;
};

VS_OUT main(VS_IN vsin)
{
	VS_OUT output;
	//output.pos = mul(float4(vsin.pos, 1.0f), mvp);
	output.pos = float4(vsin.pos, 1.0f);
	output.color = vsin.color;

	return output;
}