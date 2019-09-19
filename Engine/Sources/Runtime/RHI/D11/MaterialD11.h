#pragma once
#include "Runtime/RHI/Material.h"

namespace scarlett {

	class MaterialD11 : public Material {
	public:
		MaterialD11();
		virtual void			ApplySurface(ConstantBuffer cb);
		virtual void			ApplySkybox(ConstantBuffer cb);
	};
}