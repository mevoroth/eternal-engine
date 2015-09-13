#include "default.constants.hlsl"

struct VSIn
{
	float4 Pos : SV_Position;
	float4 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct VSOut
{
	float4 Pos : SV_Position;
	float4 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float4 WorldPos : TEXCOORD1;
};

VSOut VS( VSIn IN )
{
	VSOut OUT = (VSOut)0;
	OUT.Pos = IN.Pos;
	OUT.Pos.w = 1.f;

	OUT.Pos = mul(OUT.Pos, transpose(Model));
	OUT.Pos = mul(OUT.Pos, (View));
	OUT.Pos = mul(OUT.Pos, Projection);

	OUT.UV = IN.UV;
	OUT.Normal = IN.Normal;
	OUT.WorldPos = OUT.Pos;

	return OUT;
}
