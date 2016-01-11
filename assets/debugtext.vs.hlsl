#include "debugtext.common.hlsl"

PSIn VS(uint vid : SV_VertexID, uint pid : SV_PrimitiveID)
{
	PSIn OUT = (PSIn)0;

	uint LetterIndex = pid / 2;
	uint VertexIndex = vid % 6;
	float2 LetterSizeOnScreen = LETTER_SIZE / PositionScreenSize.zw;
	float2 LetterSizeOnFontTable = LETTER_SIZE / FontTableSize.xy;

	float2 VertexPosition[] = {
		float2(0.0f, 0.0f),
		float2(LetterSizeOnScreen.x, 0.0f),
		LetterSizeOnScreen.xy,
		float2(0.0f, LetterSizeOnScreen.y)
	};

	float2 VertexUV[] = {
		float2(0.0f, 0.0f),
		float2(LetterSizeOnScreen.x, 0.0f),
		LetterSizeOnFontTable.xy,
		float2(0.0f, LetterSizeOnScreen.y)
	};

	uint Indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	OUT.Pos = PositionScreenSize.xy + LetterSizeOnScreen.xy * (float)LetterIndex + VertexPosition[Indices[VertexIndex]];
	uint CharBlock = Line[LetterIndex / 4];
	uint OffsetInBlock = LetterIndex % 4;
	uint Char = (CharBlock >> OffsetInBlock) & 0xFF;
	OUT.UV = float2(
		(float)(Char & 0xF),
		(float)((Char >> 4) & 0xF)
	) * LetterSizeOnFontTable + VertexUV[Indices[VertexIndex]];

	return OUT;
}
