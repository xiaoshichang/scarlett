#pragma once
#include "Runtime/Interface/IResource.h"

namespace scarlett {
	enum DepthStencilStateWriteMask {
		ALL = 1,
		ZORE = 2,
	};

	enum DepthStencilStateFunc {
		NEVER = 1,
		ALWAYS = 2,
		LESS_EQUAL = 3,
		LESS = 4,
		EQUAL = 5,
		GREATER_EQUAL = 6,
		GREATER = 7,
		NOT_EQUAL = 8,
	};


	class IDepthStencilState : public IRenderResource {
	public:
		virtual void	SetWriteMask(DepthStencilStateWriteMask mask) = 0;
		virtual void	SetFunc(DepthStencilStateFunc func) = 0;

	};
}