#include "water.common.hlsl"

PSIn VS(PSIn IN)
{
	PSIn OUT = (PSIn)0;

	OUT.Pos = IN.Pos;
	OUT.Pos = mul(OUT.Pos, View);
	OUT.Pos = mul(OUT.Pos, Projection);

	OUT.UV = IN.UV;

	return OUT;
}
