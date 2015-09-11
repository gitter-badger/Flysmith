#include "TestHeader.hlsli"


float4 main(VS_OUT vsin) : SV_TARGET
{
	float3 lightPos = float3(5.0f, 5.0f, 1.0f);
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);
	float3 lightDir = normalize(lightPos - vsin.pixelPos);

	float3 color = float3(1.0f, 1.0f, 1.0f);
	
	float3 ambientStrength = 0.1f;
	float3 ambient = ambientStrength * lightColor;

	float3 diffuse = max(dot(vsin.normal, lightDir), 0.0f) * lightColor;
	
	return float4((ambient + diffuse) * color, 1.0f);
}