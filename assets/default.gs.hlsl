struct GSIn
{
	float4 Pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 Tex : TEXCOORD0;
	float4 Worldpos : TEXCOORD1;
};

struct GSOut
{
	float4 Pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 tex : TEXCOORD0;
	float4 Worldpos : TEXCOORD1;
};

[maxvertexcount(3)]
void GS(
	triangle GSIn input[3], 
	inout TriangleStream< GSOut > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOut element;
		element = input[i];
		output.Append(element);
	}
}
