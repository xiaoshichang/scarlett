#pragma once
#include "Runtime/RHI/Material.h"

namespace scarlett {

	class MaterialD11 : public Material {
	public:
		virtual void		Apply(ConstantBuffer cb) noexcept;
	};
}