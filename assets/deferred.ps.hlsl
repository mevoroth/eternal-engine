#include "lighting.hlsl"

struct PSIn
{
	float4 pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 UV : TEXCOORD0;
};

Texture2D DiffuseTexture : register(t0);
Texture2D MetallicSpecularRoughnessTexture : register(t1);
Texture2D EmissiveTexture : register(t2);
Texture2D NormalTexture : register(t3);
Texture2D WorldPositionTexture : register(t4);
Texture2D AmbientOcclusionTexture : register(t5);
SamplerState Sampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

float4 PS( PSIn IN ) : SV_TARGET
{
	//float4 sampled = WorldPositionTexture.Sample(Sampler, IN.UV);
	//return sampled / sampled.w;

	GfxBuffer GfxBufferData;
	GfxBufferData.Diffuse = DiffuseTexture.Sample(Sampler, IN.UV).xyz;
	GfxBufferData.MetallicSpecularRoughness = MetallicSpecularRoughnessTexture.Sample(Sampler, IN.UV).xyz;
	GfxBufferData.Emissive = EmissiveTexture.Sample(Sampler, IN.UV).xyz;
	GfxBufferData.Normal = NormalTexture.Sample(Sampler, IN.UV).xyz;
	GfxBufferData.AmbientOcclusion = AmbientOcclusionTexture.Sample(Sampler, IN.UV).xyz;

	return float4(ComputeShading(GfxBufferData, 0, 0), 1);
}
