#ifndef TEST_H
#define TEST_H

struct VS_IN
{
	float3 pos    : POSITION;	
	float3 normal : NORMAL;
};

struct VS_OUT
{
	float4 pos    : SV_POSITION;
	float3 normal : NORMAL;
	float3 pixelPos : PIXEL_POS;
};

#endif