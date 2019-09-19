#pragma once
#include "Runtime/RHI/Material.h"

namespace scarlett {

	class MaterialGL : public Material {
	public:
		virtual void			ApplySurface(ConstantBuffer cb);
		virtual void			ApplySkybox(ConstantBuffer cb);
	};
}
