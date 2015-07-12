#ifndef TEST_H
#define TEST_H

struct VS_IN
{
	float3 pos   : POSITION;	
	float4 color : COLOR;
};

struct VS_OUT
{
	float4 pos   : SV_POSITION;
	float4 color : COLOR;
};

#endif