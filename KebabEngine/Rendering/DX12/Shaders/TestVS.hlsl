#include "TestHeader.hlsli"


VS_OUT main(VS_IN vsin)
{
	VS_OUT output;
	output.pos = float4(vsin.pos, 0);
	output.color = vsin.color;

	return output;
}