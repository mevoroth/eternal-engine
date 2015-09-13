struct GSIn
{
	float4 Pos : SV_Position;
	float4 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float4 Worldpos : TEXCOORD1;
};

struct GSOut
{
	float4 Pos : SV_Position;
	float4 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float4 WorldPos : TEXCOORD1;
};

[maxvertexcount(3)]
void GS(
	triangle GSIn IN[3], 
	inout TriangleStream< GSOut > OUT
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOut Element;
		Element = IN[i];
		OUT.Append(Element);
	}
}
