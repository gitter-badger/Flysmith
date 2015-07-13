#include "TestHeader.hlsli"


VS_OUT main(VS_IN vsin)
{
	VS_OUT output;
	output.pos = float4(vsin.pos, 1.0f);
	output.color = vsin.color;

	return output;
}