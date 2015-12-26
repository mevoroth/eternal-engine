#ifndef _WATER_COMMON_HLSL_
#define _WATER_COMMON_HLSL_

struct VSIn
{
	float4 Pos : SV_Position;
	float2 UV : TEXCOORD0;
};

struct PSIn
{
	float4 Pos : SV_Position;
	float2 UV : TEXCOORD0;
};

cbuffer ProjectionMatrix : register(b0)
{
	matrix Projection;
	matrix View;
};

#endif
