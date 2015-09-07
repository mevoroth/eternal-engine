#include "default.constants.hlsl"

struct VSIn
{
	float4 Pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 Tex : TEXCOORD0;
	/*float4 color : COLOR;*/
};

struct VSOut
{
	float4 Pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 Tex : TEXCOORD0;
	float4 worldpos : TEXCOORD1;
};

VSOut VS( VSIn IN )
{
	VSOut OUT = (VSOut)0;
	OUT.Pos = IN.Pos;
	OUT.Pos.w = 1.f;

	//OUT.Pos = mul(OUT.Pos, Model);
	//OUT.Pos = mul(OUT.Pos, View);
	OUT.Pos = mul(OUT.Pos, transpose(Model));
	OUT.Pos = mul(OUT.Pos, (View));
	OUT.Pos = mul(OUT.Pos, Projection);

	OUT.Tex = IN.Tex;

	OUT.worldpos = OUT.Pos;

	return OUT;
}
