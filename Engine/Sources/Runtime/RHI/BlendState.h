#pragma once
#include "Runtime/Interface/IResource.h"


namespace scarlett {
	class IBlendState : public IRenderResource {
	public:
		virtual void SetEnable(bool enable) = 0;
	};
}