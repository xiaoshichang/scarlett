#pragma once

#include "Runtime/Interface/ISystem.h"

namespace scarlett {
	class World;

	class RenderDebugSystem : public IModule{
	public:
		RenderDebugSystem(World* world);
		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;

	private:
		World* mWorld;
	};

}