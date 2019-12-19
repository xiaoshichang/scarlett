#pragma once
#include "Runtime/RHI/Material.h"

namespace scarlett {

	class MaterialGL : public IMaterial {
	public:
		virtual void		Apply(ConstantBuffer cb) noexcept;
		virtual void		ApplyLight(ConstantBufferLighting cb) noexcept;
		virtual void		ApplyAnimation(ConstantBufferAnimation cb) noexcept;
	};
}
