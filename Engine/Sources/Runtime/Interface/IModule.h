#pragma once

#include "Assert.h"

namespace scarlett {

	class IModule {
	public:
		virtual int Initialize() noexcept = 0;
		virtual void Finalize() noexcept = 0;

	};

	class ITickableModule : public IModule{
		virtual void Tick() noexcept = 0;
	};
}