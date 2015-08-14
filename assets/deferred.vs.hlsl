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
};

cbuffer MatrixBuffer
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

	//output.norm = input.norm;
	OUT.tex = IN.tex;
	
	return OUT;
}
