#pragma once

#include "Runtime/Interface/IResource.h"

namespace scarlett {

	class ISamplerState : public IRenderResource {
	public:
		virtual void SetState() = 0;
	};
	
}