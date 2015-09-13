#include "lighting.hlsl"

struct PSIn
{
	float4 Pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 UV : TEXCOORD0;
};

Texture2D DiffuseTexture : register(t0);
Texture2D MetallicSpecularRoughnessTexture : register(t1);
Texture2D EmissiveTexture : register(t2);
Texture2D NormalTexture : register(t3);
Texture2D WorldPositionTexture : register(t4);
Texture2D AmbientOcclusionTexture : register(t5);
sampler DefaultSampler : register(s0);

float4 PS( PSIn IN ) : SV_Target0
{
	//float4 sampled = WorldPositionTexture.Sample(Sampler, IN.UV);
	//return sampled / sampled.w;

	GfxBuffer GfxBufferData;
	GfxBufferData.Diffuse = DiffuseTexture.Sample(DefaultSampler, IN.UV).xyz;
	GfxBufferData.MetallicSpecularRoughness = MetallicSpecularRoughnessTexture.Sample(DefaultSampler, IN.UV).xyz;
	GfxBufferData.Emissive = EmissiveTexture.Sample(DefaultSampler, IN.UV).xyz;
	GfxBufferData.Normal = NormalTexture.Sample(DefaultSampler, IN.UV).xyz;
	GfxBufferData.AmbientOcclusion = AmbientOcclusionTexture.Sample(DefaultSampler, IN.UV).xyz;

	return float4(GfxBufferData.Diffuse, 1);
	return float4(ComputeShading(GfxBufferData, 0, 0), 1);
}
