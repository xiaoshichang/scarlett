#pragma once
#include "Runtime/Interface/IResource.h"
#include <d3d11.h>

namespace scarlett{

	class DepthStencilStateD11 : public IDepthStencilState {
	public:
		DepthStencilStateD11();
		virtual void	SetWriteMask(DepthStencilStateWriteMask mask);
		virtual void	SetFunc(DepthStencilStateFunc func);
		virtual void	SafeRelease();
		virtual void	Reload();
	public:
		ID3D11DepthStencilState* mDepthStencilState;
		DepthStencilStateWriteMask mMask;
		DepthStencilStateFunc	mFunc;
	};

}