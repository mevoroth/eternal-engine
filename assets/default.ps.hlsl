struct PSIn
{
	float4 pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 tex : TEXCOORD0;
	float4 worldpos : TEXCOORD1;
};

struct PSOut
{
	float4 Diffuse						: SV_TARGET0;
	float4 MetallicSpecularRoughness	: SV_TARGET1;
	float4 Emissive						: SV_TARGET2;
	float4 Normal						: SV_TARGET3;
	float4 WorldPosition				: SV_TARGET4;
	float4 AmbiantOcclusion				: SV_TARGET5;
};

PSOut PS( PSIn IN ) : SV_TARGET
{
	PSOut OUT = (PSOut)0;

	OUT.Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
	OUT.MetallicSpecularRoughness = float4(0.f, 0.f, 0.f, 0.f);
	OUT.Emissive = float4(1.0f, 1.0f, 1.0f, 1.0f);
	OUT.Normal = float4(0.5f, 0.5f, 0.5f, 1.f);
	OUT.WorldPosition = IN.worldpos;
	OUT.WorldPosition.xyz = OUT.WorldPosition.xyz;
	OUT.AmbiantOcclusion = float4(0.5f, 0.f, 0.5f, 1.0f);

	return OUT;
}