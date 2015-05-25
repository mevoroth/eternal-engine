#ifndef _FUNCTIONS_HLSL_
#define _FUNCTIONS_HLSL_

float Square(float Value)
{
	return Value*Value;
}

float3 Square(float3 Value)
{
	return float3(
		Value.x*Value.x,
		Value.y*Value.y,
		Value.z*Value.z
	);
}

#endif
