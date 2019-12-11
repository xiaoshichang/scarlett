#pragma once
#include "Runtime/RHI/BlendState.h"
#include <d3d11.h>

namespace scarlett {

	class BlendStateD11 : public IBlendState {
	public:
		BlendStateD11();
		virtual ~BlendStateD11();

		virtual void SetEnable(bool enable);

	public:
		ID3D11BlendState*	mState;
		bool				mEnable;

	};

}