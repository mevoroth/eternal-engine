struct VSIn
{
	float4 Pos : SV_Position;
	float2 UV : TEXCOORD0;
};

struct VSOut
{
	float4 Pos : SV_Position;
	float2 UV : TEXCOORD0;
};

VSOut VS( VSIn IN )
{
	VSOut OUT = (VSOut)0;
	
	OUT.Pos = IN.Pos;
	OUT.UV = IN.UV;
	OUT.UV.y = 1 - OUT.UV.y;
	
	return OUT;
}
