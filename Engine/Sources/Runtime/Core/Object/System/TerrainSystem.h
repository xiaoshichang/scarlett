#pragma once
#include <set>

#include "Runtime/Core/Object/Components/TerrainComponent.h"
#include "Runtime/Interface/ISystem.h"

namespace scarlett {
	class World;

	class TerrainSystem : public ITickableSystem {
	public:
		TerrainSystem(World* world);

		virtual int Initialize() noexcept;
		virtual void Finalize() noexcept;
	public:
		virtual void AddComponent(TerrainComponent* comp);
		virtual void DeleteComponent(TerrainComponent* comp);
		virtual void Render();
		virtual void Tick() noexcept;

	private:
		World* mWorld;
		std::set<TerrainComponent*> mComponents;

	};
}