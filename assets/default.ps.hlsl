struct PIn
{
	float4 pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 tex : TEXCOORD0;
};

struct PSOut
{
	float4 Diffuse						: SV_TARGET0;
	float4 MetallicSpecularRoughness	: SV_TARGET1;
	float4 Emissive						: SV_TARGET2;
	float4 Normal						: SV_TARGET3;
	float4 WorldPos						: SV_TARGET4;
	float4 AmbiantOcclusion				: SV_TARGET5;
};

PSOut PS( PIn input )
{
	PSOut OUT = (PSOut)0;

	OUT.Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	OUT.MetallicSpecularRoughness = float4(0.f, 0.f, 1.f, 0.f);
	OUT.Emissive = float4(1.0f, 1.0f, 1.0f, 1.0f);
	OUT.Normal = float4(0.f, 0.f, 0.5f, 1.f);
	OUT.WorldPos = pos;

	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}