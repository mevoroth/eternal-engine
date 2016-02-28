#include "debugtext.common.hlsl"

float4 PS(PSIn IN) : SV_Target0
{
	float4 OUT = FontTableTexture.Sample(StandardSampler, IN.UV);

	OUT.xyz *= OUT.w;

	return OUT;
}
