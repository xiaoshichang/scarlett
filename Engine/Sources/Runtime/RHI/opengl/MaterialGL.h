#pragma once
#include "Runtime/RHI/Material.h"

namespace scarlett {

	class MaterialGL : public IMaterial {
	public:
		virtual void		Apply(ConstantBuffer cb) noexcept;
	};
}
