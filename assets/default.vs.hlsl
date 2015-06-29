struct VSIn
{
	float4 pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 tex : TEXCOORD0;
	/*float4 color : COLOR;*/
};

struct VSOut
{
	float4 pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 tex : TEXCOORD0;
	float4 worldpos : TEXCOORD1;
};

struct LightBuffer
{
	float3 Position;
	float Distance;
};

cbuffer LightsBuffer : register(c0)
{
	LightBuffer Lights[8];
};

cbuffer MatrixBuffer : register(c1)
{
	matrix Model;
	matrix View;
	matrix Projection;
};

VSOut VS( VSIn IN )
{
	VSOut OUT = (VSOut)0;
	OUT.pos = IN.pos;

	OUT.pos = mul(OUT.pos, Model);
	OUT.pos = mul(OUT.pos, View);
	OUT.pos = mul(OUT.pos, Projection);

	OUT.tex = IN.tex;

	OUT.worldpos = OUT.pos;

	return OUT;
}
