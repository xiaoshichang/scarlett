#pragma once
#include "Runtime/Core/Math/ScltMath.h"
#include "Runtime/Interface/IComponent.h"


namespace scarlett {

	enum LightType {
		Directional = 1,
		Point,
	};

	struct LightParamDirectional {
		Vector4f	direction;
		Vector4f	color;
	};

	struct LightParamPoint
	{
		Vector4f	position;
	};

	union LightParam
	{
		LightParam() {}
		~LightParam() {}

		LightParamDirectional	mDirectional;
		LightParamPoint			mPoint;
	};

	class LightComponent : public IComponent {

	public:
		LightComponent();
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

	public:
		LightType	mType;
		LightParam	mParam;

	};

}