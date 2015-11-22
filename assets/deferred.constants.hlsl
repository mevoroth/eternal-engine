#ifndef _DEFERRED_CONSTANTS_HLSL_
#define _DEFERRED_CONSTANTS_HLSL_

float3 LambertLighting(in float3 Normal, in float3 LightVector, in float3 DiffuseColor, in float3 LightColor)
{
	return DiffuseColor * LightColor * dot(Normal, LightVector);
}

cbuffer LightsBuffer : register(b0)
{
	LightBuffer Lights[8];
}

#endif
