#pragma once

#include "Runtime/RHI/SamplerState.h"
#include <d3d11.h>

namespace scarlett {

	class SamplerStateD11 : public SamplerState {
	public:
		SamplerStateD11();
		virtual ~SamplerStateD11();
		virtual void		Initialize() noexcept;
		virtual void		Finialize() noexcept;
	public:

		ID3D11SamplerState* m_sampleState;
	};
}