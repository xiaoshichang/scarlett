#pragma once
#include "Runtime/RHI/Material.h"

namespace scarlett {

	class MaterialD11 : public IMaterial {
	public:
		MaterialD11();

		virtual void Apply(ConstantBuffer cb) noexcept;
		void ApplySurface(ConstantBuffer cb);
		void ApplySkybox(ConstantBuffer cb);

		virtual void		ApplyLight(ConstantBufferLighting cb) noexcept;
	};
}