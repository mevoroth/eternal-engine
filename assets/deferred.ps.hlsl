struct PIn
{
	float4 pos : SV_Position;
	/*float4 norm : NORMAL;*/
	float2 tex : TEXCOORD0;
};

Texture2D DiffuseTexture : register(t0);
Texture2D MetallicSpecularRoughnessTexture : register(t1);
Texture2D EmissiveTexture : register(t2);
Texture2D NormalTexture : register(t3);
Texture2D WorldPosTexture : register(t4);
Texture2D AmbiantOcclusionTexture : register(t5);
SamplerState Sampler;

float4 PS( PIn input ) : SV_TARGET
{
	//float dotprod = input.norm.y/length(input.norm);
	//if (dotprod < 0.f)
	//{
	//	dotprod = 0.f;
	//}
	//float4 color = txDiffuse.Sample( samLinear, input.tex );
	//
	//float Ia = 0.4f;
	//float Id = 0.6f*max(0.f, dotprod);
	//float Is = 0.9f*pow(max(0.f, dotprod), 1.f);

	//color = color*Ia + color*Id + color*Is;
	//color.w = 1.f;

	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
