struct VIn
{
	float4 pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 tex : TEXCOORD0;
	/*float4 color : COLOR;*/
};

struct VOut
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

VOut VS( VIn input )
{
	VOut output = (VOut)0;
	output.pos = input.pos;
	//output.pos = mul( output.pos, WVPMat );
	output.pos = mul(output.pos, Model);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	//output.norm = input.norm;
	output.tex = input.tex;
	return output;
}
