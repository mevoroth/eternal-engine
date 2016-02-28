#include "lighting.hlsl"
#include "deferred.constants.hlsl"

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
	GfxBuffer GfxBufferData;
	GfxBufferData.Diffuse = DiffuseTexture.Sample(DefaultSampler, IN.UV).xyz;
	GfxBufferData.MetallicSpecularRoughness = MetallicSpecularRoughnessTexture.Sample(DefaultSampler, IN.UV).xyz;
	GfxBufferData.Emissive = EmissiveTexture.Sample(DefaultSampler, IN.UV).xyz;
	GfxBufferData.Normal = NormalTexture.Sample(DefaultSampler, IN.UV).xyz;
	GfxBufferData.AmbientOcclusion = AmbientOcclusionTexture.Sample(DefaultSampler, IN.UV).xyz;

	float3 FinalColor = LambertLighting(GfxBufferData.Normal, normalize(WorldPositionTexture.Sample(DefaultSampler, IN.UV) - 0), GfxBufferData.Diffuse, 0.5);
	return float4(GfxBufferData.Diffuse, 1);
	return float4(FinalColor, 1);
	return float4(GfxBufferData.Normal, 1);
	return float4(ComputeShading(GfxBufferData, 0, 0), 1);
}
