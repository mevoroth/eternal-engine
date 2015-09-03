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

struct LightBuffer
{
	float3 Position;
	float Distance;
};

cbuffer LightsBuffer : register(b0)
{
	LightBuffer Lights[8];
};

cbuffer ProjectionMatrix : register(b1)
{
	//matrix Model;
	matrix Projection;
	matrix View;
};

VSOut VS( VSIn IN )
{
	VSOut OUT = (VSOut)0;
	OUT.Pos = IN.Pos;

	//OUT.Pos = mul(OUT.Pos, Model);
	//OUT.Pos = mul(OUT.Pos, View);
	OUT.Pos = mul(OUT.Pos, Projection);
	OUT.Pos = mul(OUT.Pos, transpose(View));

	OUT.Tex = IN.Tex;

	OUT.worldpos = OUT.Pos;

	return OUT;
}
