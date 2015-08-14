struct PSIn
{
	float4 pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 tex : TEXCOORD0;
	float4 worldpos : TEXCOORD1;
};

float4 PS( PSIn IN ) : SV_TARGET
{
	return float4(1, 1, 1, 1);
}
