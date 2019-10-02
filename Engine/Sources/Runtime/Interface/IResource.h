#pragma once
#include "Runtime/Core/Math/ScltMath.h"

namespace scarlett {
	class IRenderResource{
	};

	struct ConstantBuffer
	{
		Matrix4f world;
		Matrix4f view;
		Matrix4f projection;
		Vector4f debugColor;
		Vector4f pbrParameter;
		Vector4f camPos;

		Matrix4f boneMatrix[32];
	};
}