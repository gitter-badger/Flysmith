#include "TestHeader.hlsli"


cbuffer color : register(b0)
{
	uint r;
	uint g;
	uint b;
	uint a;
};

float4 main(VS_OUT vsin) : SV_TARGET
{
	//float3 diffColor = float3(r / 255.0f, g / 255.0f, b / 255.0f);
	//float3 specColor = float3(1.0f, 1.0f, 1.0f);
	//float3 lightColor = float3(1.0f, 1.0f, 1.0f);
	//float3 lightPos = float3(1.0f, 1.0f, 1.0f);
	//float3 lightDir = normalize(vsin.pixelPos - lightPos);
	//float lightDistSq = (vsin.pixelPos - lightPos) * (vsin.pixelPos - lightPos);
	//float3 toEye = float3(0.0f, 0.0, -3.0f);

	//float diffuseLighting = saturate(dot(vsin.normal, -lightDir)); 
	//diffuseLighting *= (lightDistSq / dot(lightPos - vsin.pixelPos, lightPos - vsin.pixelPos));

	//// Using Blinn half angle modification for perofrmance over correctness
	//float3 h = normalize(normalize(toEye - vsin.pixelPos) - lightDir);

	//float specularFactor = 0.5f;
	//float specLighting = pow(saturate(dot(h, vsin.normal)), specularFactor);

	//return float4(saturate(
	//	lightColor +
	//	(diffColor * lightColor * diffuseLighting * 0.6) +
	//	(specColor * lightColor * specLighting * 0.5) 
	//	), 1);


	float3 lightPos = float3(1.0f, 1.0f, 1.0f);
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);
	float3 lightDir = normalize(lightPos - vsin.pixelPos);

	float3 color = float3(r / 255.0f, g / 255.0f, b / 255.0f);
	
	float3 ambientStrength = 0.1f;
	float3 ambient = ambientStrength * lightColor;

	float3 diffuse = max(dot(vsin.normal, lightDir), 0.0f) * lightColor;
	
	return float4((ambient + diffuse) * color, 1.0f);
}