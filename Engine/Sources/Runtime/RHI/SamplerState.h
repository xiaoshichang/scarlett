#pragma once

#include "Runtime/Interface/IResource.h"

namespace scarlett {

	class SamplerState : public ISamplerState {
	public:
		virtual void		Initialize() noexcept;
		virtual void		Finialize() noexcept;
	};
	
}