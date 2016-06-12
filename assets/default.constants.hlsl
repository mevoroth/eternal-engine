#ifndef _DEFAULT_CONSTANTS_HLSL_
#define _DEFAULT_CONSTANTS_HLSL_

cbuffer ProjectionMatrix : register(b0)
{
	//matrix Model;
	matrix Projection;
	matrix View;
};

cbuffer ObjectMatrix : register(b1)
{
	matrix Model;
};

#endif
