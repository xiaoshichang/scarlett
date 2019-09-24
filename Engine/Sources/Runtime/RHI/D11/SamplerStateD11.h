#pragma once

#include "Runtime/RHI/SamplerState.h"
#include <d3d11.h>

namespace scarlett {

	class SamplerStateD11 : public ISamplerState {
	public:
		SamplerStateD11();
		virtual ~SamplerStateD11();

		virtual void SetState();

	public:

		ID3D11SamplerState* m_sampleState;
	};
}