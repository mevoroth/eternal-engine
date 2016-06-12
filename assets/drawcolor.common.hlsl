#ifndef _DRAWCOLOR_COMMON_HLSL_
#define _DRAWCOLOR_COMMON_HLSL_

struct VSIn
{
	float4 Pos : SV_Position;
	float4 Col : COLOR0;
};

struct PSIn
{
	float4 Pos : SV_Position;
	float4 Col : COLOR0;
};

cbuffer ModelViewProjectionMatrix : register(b0)
{
	matrix ModelViewProjection;
};

#endif
