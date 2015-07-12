#include "TestHeader.hlsli"


float4 main(VS_OUT vsin) : SV_TARGET
{
	return vsin.color;
}