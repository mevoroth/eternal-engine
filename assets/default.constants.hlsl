struct LightBuffer
{
	float3 Position;
	float Distance;
};

cbuffer LightsBuffer : register(b0)
{
	LightBuffer Lights[8];
};

cbuffer ProjectionMatrix : register(b1)
{
	//matrix Model;
	matrix Projection;
	matrix View;
};

cbuffer ObjectMatrix : register(b2)
{
	matrix Model;
};
