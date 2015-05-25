#ifndef _LIGHTING_HLSL_
#define _LIGHTING_HLSL_

#include "definitions.hlsl"
#include "functions.hlsl"

struct GfxBuffer
{
	float3 Diffuse;
	float3 MetallicSpecularRoughness;
	float3 Emissive;
	float3 Normal;
	float3 AmbientOcclusion;
};

float3 Diffuse(float3 DiffuseColor)
{
	return DiffuseColor * (1.0f / PI);
}

float Distribution(float Roughness, float NdotH)
{
	float m = Roughness * Roughness;
	float m2 = m * m;
	float d = (NdotH * m2 - NdotH) * NdotH + 1.f;
	return m2 / (PI*d*d);
}

float GeometryVisibility(float Roughness, float NdotV, float NdotL)
{
	float a = Square(Roughness);
	float Vis_SmithV = NdotL * (NdotV * (1.f - a) + a);
	float Vis_SmithL = NdotV * (NdotL * (1.f - a) + a);
	return 0.5f * rcp(Vis_SmithV + Vis_SmithL);
}

float3 Fresnel(float3 SpecularColor, float VdotH)
{
	float3 SpecularColorSqrt = sqrt(clamp(float3(0.f, 0.f, 0.f), float3(0.99f, 0.99f, 0.99f), SpecularColor));
	float3 n = (1.f + SpecularColorSqrt) / (1.f - SpecularColorSqrt);
	float3 g = sqrt(n*n + VdotH*VdotH - 1.f);
	return 0.5f * Square((g - VdotH) / (g + VdotH)) * (1.f + Square(((g + VdotH)*VdotH - 1.f) / ((g - VdotH)*VdotH + 1.f)));
}

float3 ComputeShading(GfxBuffer GfxBufferData, float3 Light, float3 View)
{
	float3 Half = normalize(View + Light);
	float NdotL = saturate(dot(GfxBufferData.Normal, Light));
	float NdotH = saturate(dot(GfxBufferData.Normal, Half));
	float VdotH = saturate(dot(View, Half));
	float NdotV = saturate(dot(GfxBufferData.Normal, View));

	float Distrib = Distribution(GfxBufferData.MetallicSpecularRoughness.z, NdotH);
	float Visibility = GeometryVisibility(GfxBufferData.MetallicSpecularRoughness.z, NdotV, NdotL);
	//float Fresnel = Fresnel(GfxBufferData.Specular)

	return Diffuse(GfxBufferData.Diffuse) + Distrib * Visibility;
}

#endif
