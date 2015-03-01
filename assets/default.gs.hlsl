struct GIn
{
	float4 pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 tex : TEXCOORD0;
};

struct GOut
{
	float4 pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 tex : TEXCOORD0;
};

[maxvertexcount(3)]
void GS(
	triangle GIn input[3], 
	inout TriangleStream< GOut > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GOut element;
		element = input[i];
		output.Append(element);
	}
}
