#include "debugtext.common.hlsl"

PSIn VS(uint vid : SV_VertexID)
{
	PSIn OUT = (PSIn)0;

	uint LetterIndex = vid / 6;
	uint VertexIndex = vid % 6;
	// Optimizable
	float2 LetterSizeOnScreen = LETTER_SIZE * 2 / PositionScreenSize.zw;
	float2 LetterSizeOnFontTable = LETTER_SIZE / FontTableSize.xy;

	float2 VertexPosition[] = {
		float2(0.0f, 0.0f),
		float2(LetterSizeOnScreen.x, 0.0f),
		LetterSizeOnScreen.xy,
		float2(0.0f, LetterSizeOnScreen.y)
	};

	float2 VertexUV[] = {
		float2(0.0f, 0.0f),
		float2(LetterSizeOnFontTable.x, 0.0f),
		LetterSizeOnFontTable.xy,
		float2(0.0f, LetterSizeOnFontTable.y)
	};

	uint Indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	OUT.Pos = float4(PositionScreenSize.xy / PositionScreenSize.zw + VertexPosition[1].xy * (float)LetterIndex + VertexPosition[Indices[VertexIndex]], 0, 1) * float4(2, 2, 2, 1) + float4(-1, -1, 0, 0);
	uint CharBlock = Line[LetterIndex / 4];
	uint OffsetInBlock = LetterIndex % 4;
	uint Char = (CharBlock >> (OffsetInBlock*8));
	OUT.UV = float2(
		(float)(Char & 0xF),
		(float)((Char >> 4) & 0xF)
	) * LetterSizeOnFontTable + VertexUV[Indices[VertexIndex]];

	OUT.Pos.y *= -1;

	return OUT;
}
