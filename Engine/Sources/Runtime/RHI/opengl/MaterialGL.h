#pragma once
#include "Runtime/RHI/Material.h"

namespace scarlett {

	class MaterialGL : public Material {
	public:
		virtual void		Apply(ConstantBuffer cb) noexcept;
	};
}
