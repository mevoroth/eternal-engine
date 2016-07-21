#include "Tests.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

#include "Macros/Macros.hpp"
#include "Types/Types.hpp"

namespace Tests
{
	using namespace Eternal::Types;

	void ExecuteTest()
	{
		ETERNAL_ASSERT(ExecuteTypesTest());
	}

	bool ExecuteTypesTest()
	{
		Vector4 Position(10.f, 10.f, 10.f, 1.f);

		Matrix4x4 TranslationMatrix(
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			1.f, 2.f, 1.5f, 1.f
		);

		float cosPi2 = std::cos(M_PI_2);
		float sinPi2 = std::sin(M_PI_2);

		Matrix4x4 RotationMatrix(
			1.f, 0.f,		0.f,		0.f,
			0.f, cosPi2,	-sinPi2,	0.f,
			0.f, sinPi2,	cosPi2,		0.f,
			0.f, 0.f,		0.f,		1.f
		);
		
		Matrix4x4 ScaleMatrix(
			2.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.5f, 0.f,
			0.f, 0.f, 0.f, 1.f
		);

		Vector4 TranslatedPosition	= TranslationMatrix * Position;
		Vector4 RotatedPosition		= RotationMatrix * Position;
		Vector4 ScaledPosition		= ScaleMatrix * Position;

		ETERNAL_ASSERT(TranslatedPosition	== Vector4(11.f, 12.f, 11.5f, 1.f));
		ETERNAL_ASSERT(RotatedPosition		== Vector4(10.f, 10.f, -10.f, 1.f));
		ETERNAL_ASSERT(ScaledPosition		== Vector4(20.f, 10.f, 15.f, 1.f));

		Matrix4x4 OrthoProjectionMatrix = NewOrthoLH(1024.0f, -1024.0f, -1024.f, 1024.f, 0.f, 1024.f);

		Vector4 ProjectedPosition			= OrthoProjectionMatrix * Position;
		Vector4 ProjectedTranslatedPosition	= OrthoProjectionMatrix * TranslatedPosition;

		ETERNAL_ASSERT(ProjectedPosition			== Vector4(0.009765625f, 0.009765625f, 0.009765625f, 1.f));
		ETERNAL_ASSERT(ProjectedTranslatedPosition	== Vector4(0.0107421875f, 0.01171875f, 0.01123046875f, 1.f));

		//NewLookToLH(Vector3())

		//Vector4 RandomPosition = (TranslationMatrix * OrthoProjectionMatrix) * Position;



		return true;
	}
}
