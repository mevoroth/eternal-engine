#include "default.constants.hlsl"

struct PSIn
{
	float4 Pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 Tex : TEXCOORD0;
	float4 Worldpos : TEXCOORD1;
};

struct PSOut
{
	float4 Diffuse						: SV_TARGET0;
	float4 MetallicSpecularRoughness	: SV_TARGET1;
	float4 Emissive						: SV_TARGET2;
	float4 Normal						: SV_TARGET3;
	float4 WorldPosition				: SV_TARGET4;
	float4 AmbiantOcclusion				: SV_TARGET5;
};

float ComputeLight(in float4 L, in float3 Pos)
{
	return length(L.xyz - Pos) * length(L.xyz - Pos) / L.w;
}

PSOut PS( PSIn IN ) : SV_TARGET
{
	PSOut OUT = (PSOut)0;

	OUT.Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f) * ComputeLight(float4(Lights[0].Position, Lights[0].Distance), IN.Worldpos.xyz);
	OUT.MetallicSpecularRoughness = float4(0.f, 0.f, 0.f, 0.f);
	OUT.Emissive = float4(1.0f, 1.0f, 1.0f, 1.0f);
	OUT.Normal = float4(0.5f, 0.5f, 0.5f, 1.f);
	OUT.WorldPosition = IN.Worldpos;
	OUT.WorldPosition.xyz = OUT.WorldPosition.xyz;
	OUT.AmbiantOcclusion = float4(0.5f, 0.f, 0.5f, 1.0f);

	return OUT;
}