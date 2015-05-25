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

cbuffer MatrixBuffer : register(c0)
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
