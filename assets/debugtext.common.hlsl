#ifndef _DEBUGTEXT_COMMON_HLSL_
#define _DEBUGTEXT_COMMON_HLSL_

struct PSIn
{
	float4 Pos : SV_Position;
	float2 UV : TEXCOORD0;
};

cbuffer DebugTextParameters : register(b0)
{
	// .xy : Position
	// .zw : Screen Size
	float4 PositionScreenSize;
	float2 FontTableSize;
	uint LineLength;
};

StructuredBuffer<uint> Line;
Texture2D FontTableTexture : register(t0);
sampler StandardSampler : register(s0);

#define LETTER_SIZE	(float2(5.0f, 7.0f))

#endif
