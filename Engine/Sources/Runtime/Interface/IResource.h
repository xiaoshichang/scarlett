#pragma once
#include "Runtime/Core/Math/ScltMath.h"

namespace scarlett {
	class IRenderResource{
	};

	struct ConstantBuffer
	{
		Matrix4x4f world;
		Matrix4x4f view;
		Matrix4x4f projection;
		Vector4f debugColor;
		Vector4f pbrParameter;
		Vector4f camPos;

		Matrix4x4f boneMatrix[32];

		
	};
}